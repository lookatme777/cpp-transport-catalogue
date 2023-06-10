
#include "json_builder.h"
/*BUILDER*/
json::Node json::Builder::MNode(Node::Value value_) {
    if (std::holds_alternative<bool>(value_)) {
        return Node(std::get<bool>(value_));
    }
    else if (std::holds_alternative<int>(value_)) {
        return Node(std::get<int>(value_));
    }
    else if (std::holds_alternative<double>(value_)) {
        return Node(std::get<double>(value_));
    }
    else if (std::holds_alternative<std::string>(value_)) {
        return Node(std::move(std::get<std::string>(value_)));
    }
    else if (std::holds_alternative<Array>(value_)) {
        return Node(std::move(std::get<Array>(value_)));
    }
    else if (std::holds_alternative<Dict>(value_)) {
        return Node(std::move(std::get<Dict>(value_)));
    }
    else {
        return Node();
    }
}


void json::Builder::AddNode(Node node) {
    if (nodes_stack_.empty()) {
        if (!root_.IsNull()) {
            throw std::logic_error("root has already been added");
        }
        root_ = node;
        return;
    }
    else {
        if (!nodes_stack_.back()->IsArray() && !nodes_stack_.back()->IsString()) {
            throw std::logic_error("unable to create node");
        }
        if (nodes_stack_.back()->IsArray()) {
            Array arr = nodes_stack_.back()->AsArray();
            arr.emplace_back(node);
            nodes_stack_.pop_back();
            auto arr_ptr = std::make_unique<Node>(arr);
            nodes_stack_.emplace_back(std::move(arr_ptr));
        }
        else if (nodes_stack_.back()->IsString()) {
            std::string str = nodes_stack_.back()->AsString();
            nodes_stack_.pop_back();
            if (nodes_stack_.back()->IsMap()) {
                Dict dictionary = nodes_stack_.back()->AsMap();
                dictionary.emplace(std::move(str), node);
                nodes_stack_.pop_back();
                auto dictionary_ptr = std::make_unique<Node>(dictionary);
                nodes_stack_.emplace_back(std::move(dictionary_ptr));
            }
        }
    }
}

json::KeyItem json::Builder::Key(const std::string key) {
    if (nodes_stack_.empty()) {
        throw std::logic_error("unable to create key: stack is empty");
    }

    Node keyNode(key);
    if (!nodes_stack_.back()->IsMap()) {
        throw std::logic_error("unable to create key: current node is not a dictionary");
    }

    nodes_stack_.emplace_back(std::make_unique<Node>(std::move(keyNode)));

    return *this;
}

json::Builder& json::Builder::Value(Node::Value value)
{
    AddNode(MNode(value));
    return *this;
}

json::DictItem json::Builder::StartDict()
{
    nodes_stack_.emplace_back(std::move(std::make_unique<Node>(Dict())));
    return DictItem(*this);
}

json::ArrayItem json::Builder::StartArray()
{
    nodes_stack_.emplace_back(std::move(std::make_unique<Node>(Array())));
    return ArrayItem(*this);
}

json::Builder& json::Builder::EndDict() {
    if (nodes_stack_.empty()) {
        throw std::logic_error("Unable to close dictionary: no matching opening");
    }

    Node topNode = *nodes_stack_.back();

    if (!topNode.IsMap()) {
        throw std::logic_error("Unable to close dictionary: top node is not a dictionary");
    }

    nodes_stack_.pop_back();
    AddNode(std::move(topNode));

    return *this;
}

json::Builder& json::Builder::EndArray()
{
    if (nodes_stack_.empty()) {
        throw std::logic_error("Unable to close dictionary: no matching opening");
    }

    Node topNode = *nodes_stack_.back();

    if (!topNode.IsArray()) {
        throw std::logic_error("Unable to close array: top node is not a array");
    }

    nodes_stack_.pop_back();
    AddNode(topNode);

    return *this;
}

json::Node json::Builder::Build() {
    if (root_.IsNull()) {
        throw std::logic_error("Empty JSON");
    }

    if (!nodes_stack_.empty()) {
        throw std::logic_error("Invalid JSON");
    }

    return root_;
}
/*BUILDER*/



/*MAIN BUILD*/
json::KeyItem json::MainBuild::Key(std::string key) {
    return builder_.Key(key);
}

json::Builder& json::MainBuild::Value(Node::Value value) {
    return builder_.Value(value);
}

json::DictItem json::MainBuild::StartDict() {
    return DictItem(builder_.StartDict());
}

json::Builder& json::MainBuild::EndDict() {
    return builder_.EndDict();
}

json::ArrayItem json::MainBuild::StartArray() {
    return ArrayItem(builder_.StartArray());
}

json::Builder& json::MainBuild::EndArray() {
    return builder_.EndArray();
}
/*MAIN BUILD*/



/*KEY ITEM*/
json::DictItem  json::KeyItem::Value(Node::Value value) {
    return MainBuild::Value(std::move(value));
}
/*KEY ITEM*/



/*ARRAYITEM*/
json::ArrayItem json::ArrayItem::Value(Node::Value value) {
    return MainBuild::Value(move(value));
}
/*ARRAYITEM*/


