//
//  HashTable.h
//  CS 32 Project 4
//
//  Created by Vamsi Mokkapati on 3/9/15.
//  Copyright (c) 2015 Vamsi Mokkapati. All rights reserved.
//
#ifndef HASHTABLE_INCLUDED
#define HASHTABLE_INCLUDED

#include <iostream>
using namespace std;

template <typename KeyType,	typename ValueType>
class HashTable
{
public:
    
    HashTable(unsigned int numBuckets,	unsigned int capacity)
    {
        //initalize private variables
        m_associations = 0;
        m_capacity = capacity;
        m_numBuckets = numBuckets;
        
        //initalize array of node pointers hash table
        m_table = new Node*[numBuckets];
        //set each pointer to null
        for(int i = 0; i < numBuckets; i++)
            m_table[i] = nullptr;
        
        //head and tail start null
        head = nullptr;
        tail = nullptr;
    }
    
    ~HashTable()
    {
        RecentList* p1 = head;
        
        //delete recent list with most recently written assocations
        while(p1 != nullptr)
        {
            RecentList* temp = p1->next;
            delete p1;
            p1 = temp;
        }
        
        Node* p2;
        
        //delete all the items in the has table
        for(int i = 0; i < m_numBuckets; i++)
        {
            p2 = m_table[i];
            while(p2 != nullptr)
            {
                Node* temp = p2->next;
                delete p2;
                p2 = temp;
            }
        }
        
        head = nullptr;
        tail = nullptr;
        
        //delete pointers in hash table
        delete []m_table;
    }
    
    bool isFull()	const
    {
        //if buckets used is capacity, its full ,so return true
        if(m_numBucketsUsed == m_capacity)
            return true;
        return false;
    }
    
    bool set(const KeyType&	key,	const ValueType& value, bool permanent =	false)
    {
        //get bucket number in hash table
        int bucket = getBucketForKey(key);
        
        bool flag = false;
        
        //chcek if bucket already exits
        Node* temp = m_table[bucket];
        while(temp != nullptr)
        {
            if(temp->m_key == key)
            {
                flag = true;
                break;
            }
            temp = temp->next;
        }
        
        //if table full and bucket exists ,return false
        if(isFull() && !flag)
            return false;
        
        
        //if there are not nodes in bukcet
        if(m_table[bucket] == nullptr)
        {
            //create new node
            Node* n = new Node;
            n->m_key = key;
            n->m_value = value;
            n->next = nullptr;
            n->prev = nullptr;
            
            //bucket points to this new node cause its head
            m_table[bucket] = n;
            
            //increment buckets used
            m_numBucketsUsed++;
            
            //if its not permanetn
            if(!permanent)
            {
                //add bucket to recent items list
                RecentList* r = new RecentList;
                n->address = r;
                r->item = n;
                
                //if head is nullptr set it to this item in recent list
                if(head == nullptr)
                {
                    head = r;
                    tail = r;
                    r->next = nullptr;
                    r->prev = nullptr;
                }
                //if its not, add it to end using tail
                else
                {
                    tail->next = r;
                    r->prev = tail;
                    tail = r;
                    r->next = nullptr;
                }
                
                //increment assocaitions
                m_associations++;
                
            }
            
        }
        
        else
        {
            //go through nodes in bucket to look for key, r just go to end
            Node* temp = m_table[bucket];
            while(temp->next != nullptr && temp->m_key != key)
                temp = temp->next;
            
            //if we found the key above
            if(temp->m_key == key)
            {
                //change value
                temp->m_value = value;
                
                //make new temp recent list node, and move it to the end of the list
                //do whats right according to its current position in the list
                RecentList* r = temp->address;
                if(r == head && r == tail)
                    return true;
                else if(r == head)
                {
                    head = r->next;
                    head->prev = nullptr;
                    tail->next = r;
                    r->prev = tail;
                    r->next = nullptr;
                    tail = r;
                    return true;
                }
                else if(r == tail)
                    return true;
                else
                {
                    r->next->prev = r->prev;
                    r->prev->next = r->next;
                    tail->prev = r;
                    r->prev = tail;
                    r->next = nullptr;
                    tail = r;
                    return true;
                }
                
                
            }
            
            //if wee didnt find the key
            else
            {
                //make a new node with the key and value
                Node* n = new Node;
                n->m_key = key;
                n->m_value = value;
                n->next = nullptr;
                n->prev = temp;
                temp->next = n;
                
                //if its not permanetn
                if(!permanent)
                {
                    //add to recent list
                    RecentList* r = new RecentList;
                    n->address = r;
                    r->item = n;
                    
                    //add same as above
                    if(head == nullptr)
                    {
                        head = r;
                        tail = r;
                        r->next = nullptr;
                        r->prev = nullptr;
                    }
                    else
                    {
                        tail->next = r;
                        r->prev = tail;
                        tail = r;
                        r->next = nullptr;
                    }
                    
                    //increment assocations
                    m_associations++;
                    
                }
            }
            
            
        }
        
        return true;
    }
    
    bool get(const KeyType& key,	ValueType& value)	const
    {
        //get bucket
        int bucket = getBucketForKey(key);
        
        //set node to where the bucket in the table points
        Node* temp = m_table[bucket];
        while(temp != nullptr)
        {
            //if u find key, get the value
            if(temp->m_key == key)
            {
                value = temp->m_value;
                return true;
            }
            temp = temp->next;
        }
        
        return false;
    }
    
    bool touch(const KeyType& key)
    {
        //get bucket
        int bucket = getBucketForKey(key);
        
        //go through bucket of nodes
        Node* p = m_table[bucket];
        while(p != nullptr)
        {
            //if u find the key, and it points to a recent list node
            if(p->m_key == key)
            {
                if(p->address != nullptr)
                {
                    //put the node at the end of the list
                    RecentList* r = p->address;
                    if(r == head && r == tail)
                        return true;
                    else if(r == head)
                    {
                        head = r->next;
                        head->prev = nullptr;
                        tail->next = r;
                        r->prev = tail;
                        r->next = nullptr;
                        tail = r;
                        return true;
                    }
                    else if(r == tail)
                        return true;
                    else
                    {
                        r->next->prev = r->prev;
                        r->prev->next = r->next;
                        tail->next = r;
                        r->prev = tail;
                        r->next = nullptr;
                        tail = r;
                        return true;
                    }
                    
                }
                
            }
            p = p->next;
        }
        return false;
    }
    
    bool discard(KeyType& key,	ValueType& value)
    {
        //if assocations is 0, nothing to discard, return
        if(m_associations == 0)
            return false;
        
        //set key and value to key and value of least-reecntly written item which is head
        key = head->item->m_key;
        value = head->item->m_value;
        
        //delete that association from recent item list
        RecentList* r = head->next;
        head = nullptr;
        if(r != nullptr)
        {
            r->prev = nullptr;
            head = r;
        }
        else
            tail = nullptr;
        
        //get bucket
        int bucket = getBucketForKey(key);
        
        //go through bucket of nodes
        Node* p = m_table[bucket];
        while(p != nullptr)
        {
            //if u find the key
            if(p->m_key == key)
            {
                //depending on its position in the nodes, delete it, and do what is appropriate
                if(p ->next == nullptr && p->prev == nullptr)
                {
                    m_table[bucket] = nullptr;
                    delete p;
                    m_numBucketsUsed--;
                    m_associations--;
                    return true;
                }
                else if(p->prev == nullptr && p->next != nullptr)
                {
                    m_table[bucket] = p->next;
                    delete p;
                    m_associations--;
                    return true;
                }
                else if(p->next == nullptr && p->prev != nullptr)
                {
                    p->prev->next = nullptr;
                    delete p;
                    m_associations--;
                    return true;
                }
                else
                {
                    p->prev->next = p->next;
                    p->next->prev = p->prev;
                    delete p;
                    m_associations--;
                    return true;
                }
                
            }
            p = p->next;
        }
        
        return true;
        
    }
    
    
private:
    //	We	prevent a	HashTable from	being	copied	or	assigned	by	declaring	the
    //	copy	constructor	and	assignment	operator	private	and	not	implementing	them.
    HashTable(const HashTable&);
    HashTable& operator=(const HashTable&);
    
    int m_associations;
    int m_numBucketsUsed;
    int m_numBuckets;
    int m_capacity;
    
    
    struct RecentList;
    struct Node
    {
        KeyType m_key;
        ValueType m_value;
        Node* next;
        Node* prev;
        RecentList* address;
    };
    
    struct RecentList
    {
        Node* item;
        RecentList* next;
        RecentList* prev;
    };
    
    
    RecentList* head;
    RecentList* tail;
    
    
    Node** m_table;
    
    
    unsigned int getBucketForKey(const KeyType& key) const
    {
        // The computeHash function must be defined
        // for each type of key that we use in some
        // hash table.
        unsigned int computeHash(KeyType); // prototype
        unsigned int result = computeHash(key);
        unsigned int bucketNum = result%m_numBuckets;
        return bucketNum;
    }
    
    
};

#endif // HASHTABLE_INCLUDED