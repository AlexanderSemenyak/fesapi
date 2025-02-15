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

namespace COMMON_NS
{
	class AbstractObject;

	/**
	* This class abstracts the various sources of dataobjects contained in a DataObject Repository.
	*/
	class DataFeeder
	{		
	public:

		/**
		* Ask this feeder to resolve a partial object to a non partial one.
		*
		* @param	The partial dataobject
		* @return	The full XML content of the partial dataobject or empty string if the feeder could not resolve it.
		*/
		virtual std::string resolvePartial(AbstractObject const * partialObj) const = 0;
	};
}
