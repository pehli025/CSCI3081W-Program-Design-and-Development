#include "gtest/gtest.h"
#include "json_helper.h"
#include "drone.h"
#include "robot.h"
#include "package.h"
#include "../include/entity_base.h"
#include <EntityProject/entity.h>
#include <EntityProject/project_settings.h>
#include <EntityProject/web_scene_viewer.h>
#include <EntityProject/osm_graph_parser.h>
#include <EntityProject/graph.h>
#include <EntityProject/facade/delivery_system.h>
#include <EntityProject/entity_console_logger.h>
#include <cmath>
#include <iostream>
#include "Beeline.h"
#include "Parabolic.h"
#include "Smart.h"
#include <vector>
using namespace entity_project;
using entity_project::IEntity;
namespace csci3081 {

class StrategyTest : public ::testing::Test {
    protected:
        void SetUp() {}
};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(StrategyTest, BeelineTest) {
    Beeline b;
    std::vector<float> src{0,5,0};
    std::vector<float> dest{100,10,100};
    std::vector<std::vector<float>> beelinePath = b.GetPath(nullptr, src,dest);
    ASSERT_EQ(4,beelinePath.size());
    std::vector<std::vector<float>> expected;
    std::vector<float> expected0{0,5,0}; // Initial position
    std::vector<float> expected1{0,BEELINE_MAX_HEIGHT,0}; // Above the initial
    std::vector<float> expected2{100,BEELINE_MAX_HEIGHT,100}; // Above the dest
    std::vector<float> expected3{100,10,100}; // Goal position.

    expected.push_back(expected0);
    expected.push_back(expected1);
    expected.push_back(expected2);
    expected.push_back(expected3);

    for (int i = 0; i < expected.size(); i++) {
        for (int j = 0; j < expected.at(i).size(); j++) {
            ASSERT_EQ(expected.at(i).at(j), beelinePath.at(i).at(j));
        }
    }
}

// Tests parabolas on the X axis
TEST_F(StrategyTest, ParabolicTestX) {
    Parabolic p;
    std::vector<float> src{0,0,0};
    std::vector<float> dest{2,0,0};
    std::vector<std::vector<float>> parabolicPath = p.GetPath(nullptr,src,dest);
    ASSERT_EQ(51,parabolicPath.size());
    for (int i = 0; i <= PARABOLIC_STEP; i++) {
        std::vector<float> actual = parabolicPath.at(i);
        float xVal = 2.0f * (float) i / 50.0;
        std::vector<float> expected{xVal, -400.0f*xVal*xVal + 800.0f*xVal ,0}; // Parabola figured out manually.
        ASSERT_EQ(expected.size(), actual.size());
        for (int j = 0; j < expected.size(); j++) {
            ASSERT_NEAR(expected.at(j), actual.at(j),.001);
        }
    }
}

// Tests parabolas on the Z axis
TEST_F(StrategyTest, ParabolicTestZ) {
    Parabolic p;
    std::vector<float> src{0,0,0};
    std::vector<float> dest{0,0,2};
    std::vector<std::vector<float>> parabolicPath = p.GetPath(nullptr,src,dest);
    ASSERT_EQ(51,parabolicPath.size());
    for (int i = 0; i <= PARABOLIC_STEP; i++) {
        std::vector<float> actual = parabolicPath.at(i);
        float zVal = 2.0f * (float) i / 50.0;
        std::vector<float> expected{0, -400.0f*zVal*zVal + 800.0f*zVal ,zVal}; // Parabola figured out manually.
        ASSERT_EQ(expected.size(), actual.size());
        for (int j = 0; j < expected.size(); j++) {
            ASSERT_NEAR(expected.at(j), actual.at(j),.001);
        }
    }
}

// Tests parabolas going through XZ axes
TEST_F(StrategyTest, ParabolicTestXZ) {
    Parabolic p;
    std::vector<float> src{0,0,0};
    std::vector<float> dest{2,0,2};
    std::vector<std::vector<float>> parabolicPath = p.GetPath(nullptr,src,dest);
    ASSERT_EQ(51,parabolicPath.size());
    for (int i = 0; i <= PARABOLIC_STEP; i++) {
        std::vector<float> actual = parabolicPath.at(i);
        float xVal = 2.0f * (float) i / 50.0f;
        float zVal = 2.0f * (float) i / 50.0f;
        // sqrt(2) * i / 100. Since this parabola goes through xz at a 
        // 45 degree angle we can rotate parabola backwards to get parabola back on x axis and caluclate values there
        float derotatedX = 2.82842712475f * (float) i / 50.0f;
        std::vector<float> expected{xVal, -200.0f*derotatedX*derotatedX + 565.6854249f*derotatedX,zVal};
        for (int j = 0; j < expected.size(); j++) {
            ASSERT_NEAR(expected.at(j), actual.at(j),.001);
        }
    }
}

TEST_F(StrategyTest, SmartTest) {
    OSMGraphParser parser;
    const IGraph* graph = parser.CreateGraph("../../repo/project/data/umn.osm", "../../repo/project/data/umn-height.csv");
    Smart s;
    std::vector<float> src{0,0,0};
    std::vector<float> dest{2,0,2};
    std::vector<std::vector<float>> smartPath = s.GetPath(graph,src,dest);
    std::vector<std::vector<float>> expected = graph->GetPath(src,dest);
    ASSERT_EQ(expected.size(), smartPath.size());
    for (int i = 0; i < smartPath.size(); i++) {
        ASSERT_EQ(expected.at(i).size(),smartPath.at(i).size());
        for (int j = 0; j < expected.at(i).size(); j++) {
            ASSERT_EQ(expected.at(i).at(j), smartPath.at(i).at(j));
        }
    }
}

}  // namespace csci3081