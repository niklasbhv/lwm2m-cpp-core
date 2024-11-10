/**
 *  Copyright 2024 Niklas Meyer
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <string>
#include <sdf/sdf_cpp_core.h>
#include "lwm2m/lwm2m_to_sdf.h"
#include "lwm2m/lwm2m_cpp_edge.h"

const std::basic_string<char> LWM2M_SDF_NS = "https://onedm.org/ecosystem/oma";
const std::basic_string<char> LWM2M_NS_PREFIX = "oma";

//! This is a global pointer to the quality name current node
//! This is designed to point at the top level sdf element like
//! for example the `sdfThing` node, not a specific sdfThing
static sdf::ReferenceTreeNode* current_quality_name_node = nullptr;

//! This is a global pointer to the given name current node
//! This is designed to point at the given name of an element
//! for example the `OnOff` node, not a top level sdf element
static sdf::ReferenceTreeNode* current_given_name_node = nullptr;

//! List containing required sdf elements
//! This list gets filled while mapping and afterward appended to the corresponding sdfModel
static std::list<std::string> sdf_required_list;

void MapLwm2mResource(const lwm2m::Resource& lwm2m_resource, sdf::SdfObject& sdf_object) {
    // Append a new sdfObject node to the tree
    auto* lwm2m_resource_reference = new sdf::ReferenceTreeNode(lwm2m_resource.name);
    current_quality_name_node->AddChild(lwm2m_resource_reference);
    current_given_name_node = lwm2m_resource_reference;

    if (lwm2m_resource.operations == lwm2m::Execute) {
        // In this case the resource gets mapped to a sdfAction
        sdf::SdfAction sdf_action;

        current_given_name_node->AddAttribute("ID", lwm2m_resource.id);
        sdf_action.label = lwm2m_resource.name;
        // multiple_instances
        if (lwm2m_resource.mandatory) {
            sdf_required_list.push_back(current_given_name_node->GeneratePointer());
        }
        sdf_action.description = lwm2m_resource.description;

        sdf_object.sdf_action[lwm2m_resource.name] = sdf_action;
    } else if (lwm2m_resource.operations != lwm2m::UndefinedOperation) {
        // In this case, the operations are either R, W or RW and thus get mapped to a sdfProperty
        sdf::SdfProperty sdf_property;

        sdf_property.label = lwm2m_resource.name;
        if (lwm2m_resource.operations == lwm2m::Read) {
            sdf_property.readable = true;
            sdf_property.writable = false;
        } else if (lwm2m_resource.operations == lwm2m::Write) {
            sdf_property.readable = false;
            sdf_property.writable = true;
        } else {
            sdf_property.readable = true;
            sdf_property.writable = true;
        }
        // multiple_instances
        if (lwm2m_resource.mandatory) {
            sdf_required_list.push_back(current_given_name_node->GeneratePointer());
        }
        switch (lwm2m_resource.type) {
            case lwm2m::String:
                sdf_property.type = "string";
                break;
            case lwm2m::Integer:
                sdf_property.type = "integer";
                break;
            case lwm2m::Float:
                sdf_property.type = "number";
                break;
            case lwm2m::Boolean:
                sdf_property.type = "boolean";
                break;
            case lwm2m::Opaque:
                break;
            case lwm2m::Time:
                break;
            case lwm2m::ObjectLink:
                break;
            case lwm2m::UnsignedInteger:
                sdf_property.type = "integer";
                break;
            case lwm2m::CoreLink:
                break;
            case lwm2m::UndefinedType:
                // An undefined type is unsupported
                break;
        }
        // range enumeration
        sdf_property.unit = lwm2m_resource.units;
        sdf_property.description = lwm2m_resource.description;

        sdf_object.sdf_property[lwm2m_resource.name] = sdf_property;
    } else {
        // Otherwise the operations are undefined
    }
}

sdf::SdfObject MapLwm2mObject(const lwm2m::Object& lwm2m_object) {
    sdf::SdfObject sdf_object;

    // Append a new sdfObject node to the tree
    auto* lwm2m_object_reference = new sdf::ReferenceTreeNode(lwm2m_object.name);
    current_quality_name_node->AddChild(lwm2m_object_reference);
    current_given_name_node = lwm2m_object_reference;

    sdf_object.label = lwm2m_object.name;
    sdf_object.description = lwm2m_object.description_1;

    // TODO: Check what the purpose of the second description is
    lwm2m_object_reference->AddAttribute("Description2", lwm2m_object.description_2);
    lwm2m_object_reference->AddAttribute("ObjectID", lwm2m_object.object_id);
    lwm2m_object_reference->AddAttribute("ObjectURN", lwm2m_object.object_urn);
    lwm2m_object_reference->AddAttribute("LWM2MVersion", lwm2m_object.lwm2m_version);
    lwm2m_object_reference->AddAttribute("ObjectVersion", lwm2m_object.object_version);
    if (!lwm2m_object.multiple_instances) {
        sdf_object.max_items = 1;
    }
    if (lwm2m_object.mandatory) {
        sdf_required_list.push_back(current_given_name_node->GeneratePointer());
    }
    for (const auto& resource : lwm2m_object.resources) {
        MapLwm2mResource(resource, sdf_object);
    }

    return sdf_object;
}

sdf::NamespaceBlock GenerateNamespaceBlock() {
    sdf::NamespaceBlock namespace_block;
    namespace_block.namespaces[LWM2M_NS_PREFIX] = LWM2M_SDF_NS;
    namespace_block.default_namespace = LWM2M_NS_PREFIX;
    return namespace_block;
}

sdf::InformationBlock GenerateInformationBlock() {
    sdf::InformationBlock information_block;
    return information_block;
}

void MapLwm2mToSdf(const lwm2m::Lwm2m& lwm2m, sdf::SdfModel& sdf_model, sdf::SdfMapping& sdf_mapping) {
    // Generate the information block for the SDF model and mapping
    sdf_model.information_block = GenerateInformationBlock();
    sdf_mapping.information_block = GenerateInformationBlock();

    // Generate the namespace block for the SDF model and mapping
    sdf_model.namespace_block = GenerateNamespaceBlock();
    sdf_mapping.namespace_block = GenerateNamespaceBlock();

    // Create a new ReferenceTree
    sdf::ReferenceTree reference_tree;
    // If the LwM2M definition contains multiple objects, the resulting SDF model contains a sdfThing with multiple
    // sdfObjects. Otherwise, the SDF model contains a single sdfObject
    if (lwm2m.size() > 1) {
        // Add sdfThing to the ReferenceTree
        auto* sdf_thing_reference = new sdf::ReferenceTreeNode("sdfThing");
        reference_tree.root->AddChild(sdf_thing_reference);
        current_quality_name_node = sdf_thing_reference;

        sdf::SdfThing sdf_thing;
        // Iterate through all LwM2M objects and map them individually
        for (const auto& lwm2m_object : lwm2m) {
            sdf_thing.sdf_object[lwm2m_object.name] = MapLwm2mObject(lwm2m_object);
            // Clear the list of required elements
            sdf_required_list.clear();
            // Set the current quality name node back to the sdfThing node
            current_quality_name_node = sdf_thing_reference;
        }
        sdf_model.sdf_thing["LWM2M"] = sdf_thing;
    } else {
        // Add sdfObject to the ReferenceTree
        auto* sdf_object_reference = new sdf::ReferenceTreeNode("sdfObject");
        reference_tree.root->AddChild(sdf_object_reference);
        current_quality_name_node = sdf_object_reference;

        sdf::SdfObject sdf_object = MapLwm2mObject(lwm2m.front());
        sdf_model.sdf_object[lwm2m.front().name] = sdf_object;
    }
}
