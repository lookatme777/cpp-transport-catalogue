
#pragma once

#include "json.h"
#include <stack>
#include <string>
#include <memory>

namespace json {

    class KeyItem;
    class DictItem;
    class ArrayItem;
    class MainBuild;

    class Builder {
    public:

        Node MNode(Node::Value value_);
        void AddNode(Node node);
        Node Build();

        KeyItem Key(std::string key_);
        Builder& Value(Node::Value value);

        /*dict*/
        DictItem StartDict();
        Builder& EndDict();

        /*array*/
        ArrayItem StartArray();
        Builder& EndArray();

    private:
        Node root_;
        std::vector<std::unique_ptr<Node>>nodes_stack_;

    };

    class MainBuild {
    public:
        MainBuild(Builder& builder) : builder_(builder) {}

        KeyItem Key(std::string key);
        DictItem StartDict();
        ArrayItem StartArray();

        Builder& Value(Node::Value value);
        Builder& EndDict();
        Builder& EndArray();

    protected:
        Builder& builder_;

    };

    class KeyItem : public MainBuild {
    public:
        KeyItem(Builder& builder) : MainBuild(builder) {}

        KeyItem Key(std::string key) = delete;
        MainBuild EndDict() = delete;
        MainBuild EndArray() = delete;

        DictItem Value(Node::Value value);
    };

    class DictItem : public MainBuild {
    public:
        DictItem(Builder& builder) : MainBuild(builder) {}
        DictItem StartDict() = delete;

        ArrayItem StartArray() = delete;
        Builder& EndArray() = delete;

        Builder& Value(Node::Value value) = delete;
    };

    class ArrayItem : public MainBuild {
    public:
        ArrayItem(Builder& builder) : MainBuild(builder) {}

        KeyItem Key(std::string key) = delete;
        Builder& EndDict() = delete;
        ArrayItem Value(Node::Value value);
    };

}//end namespace json
