#include "behaviortree_cpp_v3/bt_factory.h"

//#define MANUAL_STATIC_LINKING

#ifdef MANUAL_STATIC_LINKING
#include "dummy_nodes.h"
#endif

using namespace BT;

static const char* xml_text = R"(

 <root main_tree_to_execute = "MainTree" >

     <BehaviorTree ID="MainTree">
        <Sequence name="root_sequence">
            <ApproachObject name="approach"/>
            <CheckBattery   name="battery_ok"/>
            <OpenGripper    name="open_gripper"/>
            <ApproachObject name="approach_object"/>
            <CloseGripper   name="close_gripper"/>
        </Sequence>
     </BehaviorTree>

 </root>
 )";



int main()
{

    BehaviorTreeFactory factory;



#ifdef MANUAL_STATIC_LINKING
    using namespace DummyNodes;
    factory.registerNodeType<ApproachObject>("ApproachObject");
    factory.registerSimpleCondition("CheckBattery", std::bind(CheckBattery));
    GripperInterface gripper;
    factory.registerSimpleAction("OpenGripper", std::bind(&GripperInterface::open, &gripper));
    factory.registerSimpleAction("CloseGripper", std::bind(&GripperInterface::close, &gripper));

#else

    factory.registerFromPlugin("./libdummy_nodes_dyn.so");
#endif

    auto tree = factory.createTreeFromText(xml_text);

    
    const std::string name_to_find = "open_gripper";
    auto name_checker = [&name_to_find](const TreeNode* node) -> bool {

        if (node->name().compare(name_to_find) == 0)
        {
            std::cout<<"Found it: "<<node->name()<<std::endl;
            return true;
        }
        else
        {
            std::cout<< node->name() <<std::endl;
            return false;
        }
    };

    findTreeNodeRecursively(0, tree.root_node, name_checker);
    return 0;
}

