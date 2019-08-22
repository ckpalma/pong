#include "graph.h"

#include <vector>
#include <iostream>

///////////////////////////////////// Node ////////////////////////////////////

template<typename T>
T Node<T>::getData() {
  return data_;
}

template<typename T>
void Node<T>::setData(T data) {
  data_ = data;
}

template<typename T>
std::vector<Node<T>*> Node<T>::getNeighbors() {
  return neighbors_;
}

template<typename T>
void Node<T>::addNeighbor(Node<T>* newNeighbor) {
  neighbors_.push_back(newNeighbor->reference(this));
}

template<typename T>
void Node<T>::addNeighbors(std::vector<Node<T>*> newNeighbors) {
  neighbors_.insert(neighbors_.end(),newNeighbors.begin(),newNeighbors.end());
}

template<typename T>
void Node<T>::removeNeighbor(Node<T>* delNode) {
  bool found = false;
  std::vector<Node<T>*> newNeighbors;
  for (auto it = neighbors_.begin(); it != neighbors_.end(); it++) {
    if (it != delNode) {
      newNeighbors.push_back(it);
    }
    else {
      found = true;
    }
  }
  
  neighbors_ = newNeighbors;
  
  // Debugging
  if (found) {
    std::cout << "Found Match for ("<<delNode<<") in node ("<<this<<")\n";
  }
  else {
    std::cout << "***NO MATCH FOUND***\n";
  }
}

template<typename T>
int Node<T>::getIndex() {
  return idx_;
}

template<typename T>
void Node<T>::setIndex(int idx) {
  idx_ = idx;
}

template<typename T>
void Node<T>::setAll(T data, int idx, std::vector<Node<T>*> neighbors) {
  this->setData(data);
  this->setIndex(idx);
  this->addAdjacent(neighbors);
}

template<typename T>
std::vector<Node<T>*> Node<T>::getReferences() {
  return refs_;
}

template<typename T>
void Node<T>::removeReference(Node* delNode) {
  bool found = false;
  std::vector<Node<T>*> newRefs;
  for (auto it = refs_.begin(); it != refs_.end(); it++) {
    if (it != delNode) {
      newRefs.push_back(it);
    }
    else {
      found = true;
    }
  }
  
  refs_ = newRefs;
  
  // Debugging
  if (found) {
    std::cout << "Found Match for ("<<delNode<<") in node ("<<this<<")\n";
  }
  else {
    std::cout << "***NO MATCH FOUND***\n";
  }
}

template<typename T>
Node<T>* Node<T>::reference(Node<T>* refBy) {
  refs_.push_back(refBy);
  return this;
}

//////////////////////////////////// Graph ////////////////////////////////////

template<typename T>
Node<T> Graph<T>::getNode(int idx) {
  return nodes_[idx];
}

template<typename T>
void Graph<T>::addNode(Node<T> node) {
  node.setIndex(nodes_.size());
  nodes_.push_back(node);
}

template<typename T>
void Graph<T>::removeNode(int idx) {
  Node<T> delNode = this->getNode(idx);
  
  // Delete any Nodes that refer to this node (in an edge)
  // TODO: More efficient and intuitive way for handling edges
  std::vector<Node<T>*> delNodeRefs = delNode.getReferences();
  for (auto it = delNodeRefs.begin(); it != delNodeRefs.end(); it++) {
    it->removeNeighbor(&delNode);
  }
  
  std::vector<Node<T>*> delNodeNeighs = delNode.getNeighbors();
  for (auto it = delNodeNeighs.begin(); it != delNodeNeighs.end(); it++) {
    it->removeReference(&delNode);
  }
  
  // Delete node and fix index values for nodes that follow it in nodes_
  nodes_.erase(nodes_.begin()+idx);
  int newNodeIdx = idx;
  for (auto it = nodes_.begin()+idx; it != nodes_.end(); it++) {
    it->setIndex(newNodeIdx++);
  }
}

template<typename T>
void Graph<T>::addEdge(int src, int dest) {
  this->getNode(src).addNeighbor(&(this->getNode(dest)));
}

template<typename T>
void Graph<T>::removeEdge(int src, int dest) {
  this->getNode(src).removeNeighbor(&(this->getNode(dest)));
  this->getNode(dest).removeReference(&(this->getNode(src)));
}