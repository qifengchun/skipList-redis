#ifndef NODE_H
#define NODE_H

template<typename K, typename V>
class SkipList;

template<typename K, typename V>
class Node
{
    friend class SkipList<K, V>;
public:
    Node();
    Node(K k, V v, int level);
    ~Node();

    //get方法和set方法
    K getKey() const;
    V getValue() const;
    void setValue(V v);

    //使用forward，这个指针数组，存储当前节点的下一层节点
    // 1->2->3->4
    Node<K, V> **forward;
    int node_level;

private:
    K key;
    V value;
};

#endif // NODE_H
