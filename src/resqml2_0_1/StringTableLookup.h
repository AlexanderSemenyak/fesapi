/*-----------------------------------------------------------------------
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"; you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
-----------------------------------------------------------------------*/
#pragma once

#include "../resqml2/StringTableLookup.h"

namespace RESQML2_0_1_NS
{
	/** A string table lookup. */
	class StringTableLookup final : public RESQML2_NS::StringTableLookup
	{
	public:

		/**
		 * Only to be used in partial transfer context
		 *
		 * @param [in,out]	partialObject	If non-null, the partial object.
		 *
		 * 
		 */
		DLL_IMPORT_OR_EXPORT StringTableLookup(gsoap_resqml2_0_1::eml20__DataObjectReference* partialObject) : RESQML2_NS::StringTableLookup(partialObject) {}

		/**
		 * Creates an instance of this class in a gsoap context.
		 *
		 * @param [in,out]	repo 	A repo which will manage the memory of this instance.
		 * @param 		  	guid 	The guid to set to this instance. If empty then a new guid will be
		 * 							generated.
		 * @param 		  	title	A title for the instance to create.
		 */
		StringTableLookup(COMMON_NS::DataObjectRepository* repo, const std::string & guid, const std::string & title);

		/**
		 * Creates an instance of this class by wrapping a gsoap instance.
		 *
		 * @param [in,out]	fromGsoap	If non-null, from gsoap.
		 */
		StringTableLookup(gsoap_resqml2_0_1::_resqml20__StringTableLookup* fromGsoap) : RESQML2_NS::StringTableLookup(fromGsoap) {}

		/** Destructor does nothing since the memory is managed by the gsoap context. */
		~StringTableLookup() = default;

		DLL_IMPORT_OR_EXPORT bool containsKey(int64_t key) final;

		DLL_IMPORT_OR_EXPORT uint64_t getItemCount() const final;

		DLL_IMPORT_OR_EXPORT int64_t getKeyAtIndex(uint64_t index) const final;

		DLL_IMPORT_OR_EXPORT std::string getStringValueAtIndex(uint64_t index) const final;

		DLL_IMPORT_OR_EXPORT std::string getStringValue(int64_t key) final;

		DLL_IMPORT_OR_EXPORT void addValue(const std::string & strValue, int64_t key) final;

		DLL_IMPORT_OR_EXPORT void setValue(const std::string & strValue, int64_t key) final;

		DLL_IMPORT_OR_EXPORT int64_t getMinimumValue() final;

		DLL_IMPORT_OR_EXPORT int64_t getMaximumValue() final;

		DLL_IMPORT_OR_EXPORT std::unordered_map<int64_t, std::string> getMap() const final;

		/**
		* The standard XML namespace for serializing this data object.
		*/
		DLL_IMPORT_OR_EXPORT static constexpr char const* XML_NS = "resqml20";

		/**
		* Get the standard XML namespace for serializing this data object.
		*/
		DLL_IMPORT_OR_EXPORT std::string getXmlNamespace() const final { return XML_NS; }
	};
}
