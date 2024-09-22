// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System.Collections.Generic;
using System.Reflection.Metadata;

using Internal.TypeSystem;
using Internal.TypeSystem.Ecma;

namespace ILCompiler
{
    /// <summary>
    /// Computes a set of roots based on managed and unmanaged methods exported from a module.
    /// </summary>
    public class UnmanagedEntryPointsRootProvider : ICompilationRootProvider
    {
        private EcmaModule _module;
        private readonly NameMangler _nameMangler;

        public UnmanagedEntryPointsRootProvider(EcmaModule module, NameMangler nameMangler)
        {
            _module = module;
            _nameMangler = nameMangler;
        }

        public IEnumerable<EcmaMethod> ExportedMethods
        {
            get
            {
                MetadataReader reader = _module.MetadataReader;
                MetadataStringComparer comparer = reader.StringComparer;
                HashSet<EcmaMethod> hacks = new HashSet<EcmaMethod>();

                foreach (CustomAttributeHandle caHandle in reader.CustomAttributes)
                {
                    CustomAttribute ca = reader.GetCustomAttribute(caHandle);
                    if (ca.Parent.Kind != HandleKind.MethodDefinition)
                        continue;

                    if (!reader.GetAttributeNamespaceAndName(caHandle, out StringHandle nsHandle, out StringHandle nameHandle))
                        continue;

                    if (comparer.Equals(nameHandle, "RuntimeExportAttribute")
                        && comparer.Equals(nsHandle, "System.Runtime"))
                    {
                        var method = (EcmaMethod)_module.GetMethod(ca.Parent);
                        if (method.GetRuntimeExportName() != null)
                            yield return method;
                    }

                    if (comparer.Equals(nameHandle, "UnmanagedCallersOnlyAttribute")
                        && comparer.Equals(nsHandle, "System.Runtime.InteropServices"))
                    {
                        var method = (EcmaMethod)_module.GetMethod(ca.Parent);
                        if (method.GetUnmanagedCallersOnlyExportName(_nameMangler) != null)
                            yield return method;
                    }

                    if (comparer.Equals(nameHandle, "DynamicDependencyAttribute")
                        && comparer.Equals(nsHandle, "System.Diagnostics.CodeAnalysis"))
                    {
                        var method = (EcmaMethod)_module.GetMethod(ca.Parent);

                        // Don't add the method for each attribute.
                        if (method.HasDynamicDependencyMemberSignatureForJsExport() && !hacks.Contains(method))
                        {
                            hacks.Add(method);
                            yield return method;
                        }
                    }
                }
            }
        }

        public void AddCompilationRoots(IRootingServiceProvider rootProvider, NameMangler nodeFactoryNameMangler)
        {
            foreach (var ecmaMethod in ExportedMethods)
            {
                if (ecmaMethod.IsUnmanagedCallersOnly)
                {
                    string unmanagedCallersOnlyExportName = ecmaMethod.GetUnmanagedCallersOnlyExportName(nodeFactoryNameMangler);
                    rootProvider.AddCompilationRoot((MethodDesc)ecmaMethod, "Native callable", unmanagedCallersOnlyExportName);
                }
                else if (ecmaMethod.HasDynamicDependencyMemberSignatureForJsExport())
                {
                    string name = nodeFactoryNameMangler.GetMangledMethodName(ecmaMethod).ToString();
                    name = ecmaMethod.GetDynamicDependencyMemberSignatureForJsExportExportName(name);
                    rootProvider.AddCompilationRoot(ecmaMethod, "Native callable", name);
                }
                else
                {
                    string runtimeExportName = ecmaMethod.GetRuntimeExportName();
                    rootProvider.AddCompilationRoot((MethodDesc)ecmaMethod, "Runtime export", runtimeExportName);
                }
            }
        }
    }
}
