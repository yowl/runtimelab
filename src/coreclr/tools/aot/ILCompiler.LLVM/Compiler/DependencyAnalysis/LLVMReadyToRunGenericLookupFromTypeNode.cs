// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using Internal.TypeSystem;

namespace ILCompiler.DependencyAnalysis
{
    internal class LLVMReadyToRunGenericLookupFromTypeNode : ReadyToRunGenericLookupFromTypeNode
    {
        public LLVMReadyToRunGenericLookupFromTypeNode(NodeFactory factory, ReadyToRunHelperId helperId, object target, TypeSystemEntity dictionaryOwner)
            : base(factory, helperId, target, dictionaryOwner)
        {
            if (target.ToString() ==
                "[S.P.CoreLib]System.Array`1+ArrayEnumerator<System.Collections.Generic.LowLevelDictionary`2+Entry<Internal.Runtime.TypeLoader.TypeLoaderEnvironment+LazyDictionaryContext,native int>>.System.Collections.IEnumerator.get_Current()")
            {

            }


        }

        public override ObjectData GetData(NodeFactory factory, bool relocsOnly)
        {
            // this code for this node is written out in LLVMObjectWriter.GetCodeForReadyToRunGenericHelper
            return new ObjectData(new byte[0], new Relocation[0], 1, new ISymbolDefinitionNode[0]);
        }
    }
}
