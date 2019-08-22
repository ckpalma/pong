#include <vector>

template<typename T>
class Node {
  public:
    T getData();
    void setData(T data);
    std::vector<Node*> getNeighbors();
    void addNeighbor(Node* newNeighbor);
    void addNeighbors(std::vector<Node*> newNeighbors);
    void removeNeighbor(Node* delNode);
    int getIndex();
    void setIndex(int idx);
    void setAll(T data, int idx, std::vector<Node*> neighbors);
    std::vector<Node*> getReferences();
    void removeReference(Node* delNode);
    
  
  private:
    T data_;
    int idx_;
    std::vector<Node*> neighbors_;
    std::vector<Node*> refs_;
    Node* reference(Node* refBy);
};

template<typename T>
class Graph {
  public:
    Node<T> getNode(int idx);
    void addNode(Node<T> node);
    void removeNode(int idx);
    void addEdge(int src, int dest);
    void removeEdge(int src, int dest);
  
  private:
    std::vector<Node<T>> nodes_;
};