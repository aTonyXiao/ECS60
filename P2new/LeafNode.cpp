#include <iostream>
#include <climits>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()

void LeafNode::addToLeft(int value, int last)
{
  leftSibling->insert(values[0]);

  for(int i = 0; i < count - 1; i++)
    values[i] = values[i + 1];

  values[count - 1] = last;
  if(parent)
    parent->resetMinimum(this);
} // LeafNode::ToLeft()

void LeafNode::addToRight(int value, int last)
{
  rightSibling->insert(last);

  if(value == values[0] && parent)
    parent->resetMinimum(this);
} // LeafNode::addToRight()

void LeafNode::addToThis(int value)
{
  int i;

  for(i = count - 1; i >= 0 && values[i] > value; i--)
      values[i + 1] = values[i];

  values[i + 1] = value;
  count++;

  if(value == values[0] && parent)
    parent->resetMinimum(this);
} // LeafNode::addToThis()


void LeafNode::addValue(int value, int &last)
{
  int i;

  if(value > values[count - 1])
    last = value;
  else
  {
    last = values[count - 1];

    for(i = count - 2; i >= 0 && values[i] > value; i--)
      values[i + 1] = values[i];
    // i may end up at -1
    values[i + 1] = value;
  }
} // LeafNode:: addValue()


int LeafNode::getMaximum()const
{
  if(count > 0)  // should always be the case
    return values[count - 1];
  else
    return INT_MAX;
} // getMaximum()


int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()


LeafNode* LeafNode::insert(int value)
{
  int last;

  if(count < leafSize)
  {
    addToThis(value);
    return NULL;
  } // if room for value

  addValue(value, last);

  if(leftSibling && leftSibling->getCount() < leafSize)
  {
    addToLeft(value, last);
    return NULL;
  }
  else // left sibling full or non-existent
    if(rightSibling && rightSibling->getCount() < leafSize)
    {
      addToRight(value, last);
      return NULL;
    }
    else // both siblings full or non-existent
      return split(value, last);
}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()


LeafNode* LeafNode::remove(int value)
{   // To be written by students
  int pos = 0;
    
    while(pos < count && values[pos] != value)
    {
        pos++;
    }
    
    
    

//  for(pos = 0; pos < count && values[pos] != value; pos++);

  if(pos < count) // found value
  {
    --count;
      
      int i = pos;
      while (i < count)
      {
          values[i] = values[i + 1];
          i++;
      }
      
    
    if(count < (leafSize + 1)/ 2)
    {
      if(leftSibling)
        return rmLSibling();
      else
        if(rightSibling)
          return rmRSibling(pos);
    } // if below minimum

    if(pos == 0  && parent)
      parent->resetMinimum(this);
  }else{}

  return NULL;  // unmerged signal
}  // LeafNode::remove()


LeafNode* LeafNode::rmLSibling()
{
  if(leftSibling->getCount() <= (leafSize + 1) / 2)
  {
      
      
      int i = 0;
      while (i < count)
      {
      leftSibling->insert(values[i]);
          i++;
      }
          
      
      leftSibling->setRightSibling(rightSibling);
      
      if(rightSibling)
          rightSibling->setLeftSibling(leftSibling);
      
      return this;
  }
  else
  {
      insert(leftSibling->getMaximum());
      leftSibling->remove(values[0]);
      
      if(parent)
          parent->resetMinimum(this);
      
      return NULL;
  }
}


LeafNode* LeafNode::rmRSibling(int pos)
{
    
    
    
  if(rightSibling->getCount() <= (leafSize + 1) / 2)
  {
      
      int i = 0;
      while (i<count)
      {
          rightSibling->insert(values[i]);
          i++;
      
      }
          
      
      rightSibling->setLeftSibling(leftSibling);
      
      if(leftSibling)
          leftSibling->setRightSibling(rightSibling);
      return this;
     
    
  }
  else
  {
      insert(rightSibling->getMinimum());
      rightSibling->remove(values[count - 1]);
      if(pos == 0)
          parent->resetMinimum(this);
      return NULL;
  }
}


LeafNode* LeafNode::split(int value, int last)
{
  LeafNode *ptr = new LeafNode(leafSize, parent, this, rightSibling);


  if(rightSibling)
    rightSibling->setLeftSibling(ptr);

  rightSibling = ptr;

  for(int i = (leafSize + 1) / 2; i < leafSize; i++)
    ptr->values[ptr->count++] = values[i];

  ptr->values[ptr->count++] = last;
  count = (leafSize + 1) / 2;

  if(value == values[0] && parent)
    parent->resetMinimum(this);
  return ptr;
} // LeafNode::split()

