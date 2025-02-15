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
#include "GridConnectionSetOnPartialGridSet.h"
#include "../catch.hpp"

#include "resqml2/AbstractIjkGridRepresentation.h"
#include "resqml2/ContinuousProperty.h"
#include "resqml2/GridConnectionSetRepresentation.h"

#include "resqml2_0_1/FaultInterpretation.h"
#include "resqml2_0_1/HorizonInterpretation.h"
#include "resqml2_0_1/UnstructuredGridRepresentation.h"

using namespace std;
using namespace COMMON_NS;
using namespace resqml2_test;
using namespace RESQML2_NS;

const char* GridConnectionSetOnPartialGridSet::defaultUuid = "a6fa81a3-f703-4026-9b28-606311761235";
const char* GridConnectionSetOnPartialGridSet::defaultTitle = "Grid Connection Set multi grids";
const char* GridConnectionSetOnPartialGridSet::unstructuredGridUuid = "34073470-145e-4923-abe0-def81092b174";
const char* GridConnectionSetOnPartialGridSet::unstructuredGridTitle = "Partial Unstructured Grid";

GridConnectionSetOnPartialGridSet::GridConnectionSetOnPartialGridSet(const string & repoPath)
	: commontest::AbstractTest(repoPath) {
}

void GridConnectionSetOnPartialGridSet::initRepo() {
	// getting the hdf proxy
	EML2_NS::AbstractHdfProxy* hdfProxy = repo->getHdfProxySet()[0];

	// Unstructured grid
	RESQML2_NS::UnstructuredGridRepresentation* partialGrid = repo->createPartial<RESQML2_0_1_NS::UnstructuredGridRepresentation>(unstructuredGridUuid, unstructuredGridTitle);
	REQUIRE( partialGrid != nullptr );
	auto* propertyKind = repo->createPropertyKind("4a305182-221e-4205-9e7c-a36b06fa5b3d", "length", gsoap_eml2_3::eml23__QuantityClassKind::length);
	RESQML2_NS::ContinuousProperty* continuousProperty = repo->createContinuousProperty(partialGrid, "62d7e07d-5e17-4e42-b4b2-a4c26204cba2", "Continuous prop on partial unstructured grid", 1,
		gsoap_eml2_3::eml23__IndexableElement::cells, 
		gsoap_resqml2_0_1::resqml20__ResqmlUom::m, 
		propertyKind);
	double continuousProp1Values[6] = { 0, 1, 2, 3, 4, 5 };
	continuousProperty->pushBackDoubleHdf5Array1dOfValues(continuousProp1Values, 6, hdfProxy);

	// IJK grid
	RESQML2_NS::AbstractIjkGridRepresentation* partialIjkGrid = repo->createPartialIjkGridRepresentation("b0ec8bf4-9b93-428b-a814-87c38887f6d0", "Partial Ijk Grid");
	REQUIRE(partialIjkGrid != nullptr);
	RESQML2_NS::ContinuousProperty* continuousPropertyOnIjk = repo->createContinuousProperty(partialIjkGrid, "b20299b9-6881-4b91-ae2f-a87213437dce", "Continuous prop on partial ijk grid", 1,
		gsoap_eml2_3::eml23__IndexableElement::cells,
		gsoap_resqml2_0_1::resqml20__ResqmlUom::m,
		propertyKind);
	double continuousPropOnIjkValues[6] = { 0, 1, 2, 3, 4, 5 };
	continuousPropertyOnIjk->pushBackDoubleHdf5Array1dOfValues(continuousPropOnIjkValues, 6, hdfProxy);

	// Truncated IJK grid
	RESQML2_NS::AbstractIjkGridRepresentation* partialTruncIjkGrid = repo->createPartialTruncatedIjkGridRepresentation("def167fb-89b2-45bc-92ff-01d228142350", "Partial Truncated Ijk Grid");
	REQUIRE(partialIjkGrid != nullptr);
	RESQML2_NS::ContinuousProperty* continuousPropertyOnTruncIjk = repo->createContinuousProperty(partialTruncIjkGrid, "4caa8e9a-00b3-40c2-9460-72cb8790393a", "Continuous prop on partial truncated ijk grid", 1,
		gsoap_eml2_3::eml23__IndexableElement::cells,
		gsoap_resqml2_0_1::resqml20__ResqmlUom::m,
		propertyKind);
	double continuousPropOnTruncIjkValues[6] = { 0, 1, 2, 3, 4, 5 };
	continuousPropertyOnTruncIjk->pushBackDoubleHdf5Array1dOfValues(continuousPropOnTruncIjkValues, 6, hdfProxy);

	// Partial fault
	RESQML2_NS::FaultInterpretation* partialFaultInterp = repo->createPartial<RESQML2_0_1_NS::FaultInterpretation>("83504f50-9301-4565-9615-44099cc73ae3", "Partial Fault");
	REQUIRE(partialFaultInterp != nullptr);

	// Partial Horizon
	RESQML2_NS::HorizonInterpretation* partialHorizonInterp = repo->createPartial<RESQML2_0_1_NS::HorizonInterpretation>("e175316e-9f01-4358-87f5-2d6f4e2595b5", "Partial Horizon");

	// Grid Connection Set on one grid
	RESQML2_NS::GridConnectionSetRepresentation* gcs = repo->createGridConnectionSetRepresentation("c0214c71-eed8-4ea2-9de4-f7508caeb3c6", "Single grid gcs");
	gcs->pushBackSupportingGridRepresentation(partialGrid);
	int64_t cellConn[6] = {
		0, 1, 2, 3, 1, 3 
	};
	gcs->setCellIndexPairs(3, cellConn, -1, hdfProxy);
	gcs->pushBackInterpretation(partialFaultInterp);
	gcs->pushBackInterpretation(partialHorizonInterp);
	uint64_t cumulative[3] = { 1, 2, 3 };
	int64_t interpIndex[3] = { 0, 1, -1 };
	gcs->setConnectionInterpretationIndices(cumulative, interpIndex, hdfProxy);

	// Grid Connection Set on several grid
	RESQML2_NS::GridConnectionSetRepresentation* gcsMultiGrids = repo->createGridConnectionSetRepresentation(defaultUuid, defaultTitle);
	gcsMultiGrids->pushBackSupportingGridRepresentation(partialGrid);
	gcsMultiGrids->pushBackSupportingGridRepresentation(partialIjkGrid);
	gcsMultiGrids->pushBackSupportingGridRepresentation(partialTruncIjkGrid);
	int64_t cellConnMultiGrids[6] = {
		1, 2, 1, 2, 1, 2
	};
	uint16_t multiGridIndices[6] = {
		0, 0, 1, 1, 1, 2
	};
	gcsMultiGrids->setCellIndexPairs(3, cellConnMultiGrids, -1, hdfProxy, (std::numeric_limits<unsigned short>::max)(), multiGridIndices);
	gcsMultiGrids->setInterpretationForAllConnections(partialFaultInterp);
}

void GridConnectionSetOnPartialGridSet::readRepo() {
	// getting the ContinuousProperty
	RESQML2_NS::GridConnectionSetRepresentation* gcsSingleGrid = repo->getDataObjectByUuid<RESQML2_NS::GridConnectionSetRepresentation>("c0214c71-eed8-4ea2-9de4-f7508caeb3c6");
	RESQML2_NS::GridConnectionSetRepresentation* gcsMultiGrids = repo->getDataObjectByUuid<RESQML2_NS::GridConnectionSetRepresentation>(defaultUuid);

	// checking that the supporting representation is partial
	REQUIRE(!gcsSingleGrid->isBasedOnMultiGrids());
	REQUIRE(gcsSingleGrid->getSupportingGridRepresentationCount() == 1);
	REQUIRE(gcsSingleGrid->getSupportingGridRepresentation(0)->isPartial());
	REQUIRE(gcsSingleGrid->getCellIndexPairCount() == 3);
	{
		int64_t cellIndexPair[6];
		gcsSingleGrid->getCellIndexPairs(cellIndexPair);
		REQUIRE(cellIndexPair[0] == 0);
		REQUIRE(cellIndexPair[1] == 1);
		REQUIRE(cellIndexPair[2] == 2);
		REQUIRE(cellIndexPair[3] == 3);
		REQUIRE(cellIndexPair[4] == 1);
		REQUIRE(cellIndexPair[5] == 3);
	}
	REQUIRE(gcsSingleGrid->isAssociatedToInterpretations());
	REQUIRE(gcsSingleGrid->getInterpretationCount() == 2);
	REQUIRE(gcsSingleGrid->getCellIndexPairCountFromInterpretationIndex(0) == 1);
	REQUIRE(gcsSingleGrid->getCellIndexPairCountFromInterpretationIndex(1) == 1);
	int64_t cellConn[2];
	gcsSingleGrid->getGridConnectionSetInformationFromInterpretationIndex(cellConn, nullptr, nullptr, 0);
	REQUIRE(cellConn[0] == 0);
	REQUIRE(cellConn[1] == 1);
	RESQML2_NS::AbstractFeatureInterpretation* interp = gcsSingleGrid->getInterpretationFromIndex(0);
	REQUIRE(dynamic_cast<RESQML2_0_1_NS::FaultInterpretation*>(interp) != nullptr);
	REQUIRE(interp->getTitle() == "Partial Fault");
	gcsSingleGrid->getGridConnectionSetInformationFromInterpretationIndex(cellConn, nullptr, nullptr, 1);
	REQUIRE(cellConn[0] == 2);
	REQUIRE(cellConn[1] == 3);
	interp = gcsSingleGrid->getInterpretationFromIndex(1);
	REQUIRE(dynamic_cast<RESQML2_0_1_NS::HorizonInterpretation*>(interp) != nullptr);
	REQUIRE(interp->getTitle() == "Partial Horizon");

	REQUIRE(gcsMultiGrids->isBasedOnMultiGrids());
	REQUIRE(gcsMultiGrids->getSupportingGridRepresentationCount() == 3);
	REQUIRE(gcsMultiGrids->getSupportingGridRepresentation(0)->isPartial());
	REQUIRE(gcsMultiGrids->getSupportingGridRepresentation(1)->isPartial());
	REQUIRE(gcsMultiGrids->getSupportingGridRepresentation(2)->isPartial());
	REQUIRE(gcsMultiGrids->getCellIndexPairCount() == 3);
	REQUIRE(gcsMultiGrids->isAssociatedToInterpretations());
	{
		int64_t cellIndexPair[6];
		gcsMultiGrids->getCellIndexPairs(cellIndexPair);
		REQUIRE(cellIndexPair[0] == 1);
		REQUIRE(cellIndexPair[1] == 2);
		REQUIRE(cellIndexPair[2] == 1);
		REQUIRE(cellIndexPair[3] == 2);
		REQUIRE(cellIndexPair[4] == 1);
		REQUIRE(cellIndexPair[5] == 2);
	}
	uint16_t gridIndexPair[6];
	gcsMultiGrids->getGridIndexPairs(gridIndexPair);
	REQUIRE(gridIndexPair[0] == 0);
	REQUIRE(gridIndexPair[1] == 0);
	REQUIRE(gridIndexPair[2] == 1);
	REQUIRE(gridIndexPair[3] == 1);
	REQUIRE(gridIndexPair[4] == 1);
	REQUIRE(gridIndexPair[5] == 2);

	REQUIRE(gcsMultiGrids->isAssociatedToInterpretations());
	REQUIRE(gcsMultiGrids->getInterpretationCount() == 1);
	REQUIRE(gcsMultiGrids->getCellIndexPairCountFromInterpretationIndex(0) == 3);
	int64_t cellIndexPair[6];
	gcsMultiGrids->getGridConnectionSetInformationFromInterpretationIndex(cellIndexPair, nullptr, nullptr, 0);
	REQUIRE(cellIndexPair[0] == 1);
	REQUIRE(cellIndexPair[1] == 2);
	REQUIRE(cellIndexPair[2] == 1);
	REQUIRE(cellIndexPair[3] == 2);
	REQUIRE(cellIndexPair[4] == 1);
	REQUIRE(cellIndexPair[5] == 2);
}
