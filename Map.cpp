#include "Map.h"

Map::Map()
    : m_size(0)
{
    headPtr = nullptr;
    tailPtr = nullptr;
}

Map::Map(const Map& src) {
    m_size = src.size();

    headPtr = nullptr;
    tailPtr = nullptr;

    if (src.size() > 0) {
        Node* p = src.headPtr;
        while (p != nullptr) {
            insertInOrder(p->m_key, p->m_value);
            p = p->nextNode;
        }
    }
}

Map& Map::operator=(const Map& src) {

    if (&src == this) return *this;

    Node* p;
    p = headPtr;

    while (p != nullptr) {
        Node* n = p->nextNode;
        delete p;
        p = n;
    }

    headPtr = nullptr;
    tailPtr = nullptr;

    m_size = src.size();

    if (src.size() > 0) {
        Node* p = src.headPtr;
        while (p != nullptr) {
            insertInOrder(p->m_key, p->m_value);
            p = p->nextNode;
        }
    }
    return *this;
}

bool Map::erase(const KeyType& key)
{
    /* Linked List Implementation */
    if (headPtr == nullptr) return false;
    if (!contains(key)) return false;

    if (headPtr->m_key == key) {
        Node* kill = headPtr;
        headPtr = kill->nextNode;
        if (m_size > 1) kill->nextNode->previousNode = nullptr;
        delete kill;
        m_size--;
        return true;
    }
    if (tailPtr->m_key == key) {
        Node* kill = tailPtr;
        tailPtr = kill->previousNode;
        if (m_size > 1) kill->previousNode->nextNode = nullptr;
        delete kill;
        m_size--;
        return true;
    }

    Node* p = headPtr;

    while (p != nullptr) {
        if (p->nextNode->m_key == key && p->nextNode != nullptr) break;
        p = p->nextNode;
    }
    if (p != nullptr) {
        Node* kill = p->nextNode;
        p->nextNode = kill->nextNode;
        kill->nextNode->previousNode = p;
        delete kill;
        m_size--;
        return true;
    }
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    /* Linked List Implementation */
    if (!contains(key)) return false;
    value = getValueFromKey(key);
    return true;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    /* Linked List Implementation */
    Node* p = headPtr;

    if (i < 0 || i >= m_size) return false;
    while (i > 0) {
        p = p->nextNode;
        i--;
    }
    key = p->m_key;
    value = p->m_value;
    return true;
}

void Map::swap(Map& other)
{
    /* Linked List Implementation */

    //Swap head pointers
    Node* tempPtr = headPtr;
    headPtr = other.headPtr;
    other.headPtr = tempPtr;

    //Swap tail pointers
    tempPtr = tailPtr;
    tailPtr = other.tailPtr;
    other.tailPtr = tempPtr;

    //Swap sizes
    int tempSize = m_size;
    m_size = other.m_size;
    other.m_size = tempSize;
}

bool Map::doInsertOrUpdate(const KeyType& key, const ValueType& value,
    bool mayInsert, bool mayUpdate)
{
    /* Linked List Implementation */
    if (contains(key)) {
        if (mayUpdate) {
            //change the value of the pair to value
            Node* p = headPtr;

            while (p != nullptr) {
                if (p->m_key == key) {
                    p->m_value = value;
                    return true;
                }
                p = p->nextNode;
            }
        }
        return mayUpdate;
    }
    if (!mayInsert) {
        return false;
    }

    //Insert Value
    insertInOrder(key, value);
    m_size++;
    return true;
}

bool merge(const Map& m1, const Map& m2, Map& result) {
    bool returnValue = true;
    KeyType key1;
    ValueType value1;

    if (&m1 == &result) {
        ValueType value2;
        for (int i = 0; i < m2.size(); i++) {
            m2.get(i, key1, value1);
            if (!m1.contains(key1)) {
                result.insert(key1, value1);
            }
            else {
                m1.get(key1, value2);
                if (value2 != value1) {
                    returnValue = false;
                    result.erase(key1);
                }
            }
        }
        return returnValue;
    }
    else if (&m2 == &result) {
        ValueType value2;
        for (int i = 0; i < m1.size(); i++) {
            m1.get(i, key1, value1);
            if (!m2.contains(key1)) {
                result.insert(key1, value1);
            }
            else {
                m2.get(key1, value2);
                if (value2 != value1) {
                    returnValue = false;
                    result.erase(key1);
                }
            }
        }
        return returnValue;
    }

    Map blank;
    result = blank;

    for (int i = 0; i < m1.size(); i++) {
        m1.get(i, key1, value1);
        if (!m2.contains(key1)) {
            result.insert(key1, value1);
        }
        else {
            ValueType value2;
            m2.get(key1, value2);
            if (value2 != value1) {
                returnValue = false;
            }
            else {
                result.insert(key1, value1);
            }
        }
    }

    for (int i = 0; i < m2.size(); i++) {
        m2.get(i, key1, value1);
        if (!m1.contains(key1)) {
            result.insert(key1, value1);
        }
    }

    return returnValue;
}

void reassign(const Map& m, Map& result) {
    result = m;
    if (result.size() > 1) {
        KeyType firstKey;
        ValueType firstValue;
        result.get(0, firstKey, firstValue);

        KeyType currentKey;
        ValueType currentValue;

        KeyType nextKey;
        ValueType nextValue;

        for (int i = 0; i < result.size() - 1; i++) {
            result.get(i, currentKey, currentValue);
            result.get(i + 1, nextKey, nextValue);
            result.update(currentKey, nextValue);
        }
        result.get(result.size() - 1, currentKey, currentValue);
        result.update(currentKey, firstValue);

    }
}

Map::~Map() {
    Node* p;
    p = headPtr;

    while (p != nullptr) {
        Node* n = p->nextNode;
        delete p;
        m_size--;
        p = n;
    }
    headPtr = nullptr;
    tailPtr = nullptr;
}

//Linked List Helper Functions

void Map::addToFront(const KeyType& key, const ValueType& value) {
    //Initialize node
    Node* p;
    p = new Node;
    p->m_key = key;
    p->m_value = value;

    //Make connections
    p->previousNode = nullptr;
    p->nextNode = headPtr;

    //Case if is first element created in list
    if (headPtr == nullptr) tailPtr = p;
    headPtr = p;
}

void Map::addToRear(const KeyType& key, const ValueType& value) {
    //Initialize node
    Node* p;
    p = new Node;
    p->m_key = key;
    p->m_value = value;

    //Make connections
    tailPtr->nextNode = p;
    p->previousNode = tailPtr;
    p->nextNode = nullptr;

    //Case if is first element created in list
    if (tailPtr == nullptr) headPtr = p;
    tailPtr = p;
}

void Map::insertInOrder(const KeyType& key, const ValueType& value) {
    if (!contains(key)) {
        if (headPtr == nullptr) {
            addToFront(key, value);
        }
        else if (headPtr->m_key > key) {
            addToFront(key, value);
        }
        else if (tailPtr->m_key < key) {
            addToRear(key, value);
        }
        else {
            //Add pair to list in order related to key
            Node* p = headPtr;

            while (p->nextNode != nullptr) {
                if (p->nextNode->m_key > key) break;
                p = p->nextNode;
            }

            Node* latest = new Node;
            latest->m_key = key;
            latest->m_value = value;
            latest->previousNode = p;
            latest->nextNode = p->nextNode;

            p->nextNode = latest;
            if (latest->nextNode != nullptr) latest->nextNode->previousNode = latest;
        }
    }
}
ValueType Map::getValueFromKey(const KeyType& key) const{
    //Add pair to list in order related to key
    Node* p = headPtr;

    while (p != nullptr) {
        if (p->m_key == key) {
            return p->m_value;
        }
        p = p->nextNode;
    }
    return m_size;
}


bool Map::findItem(const KeyType& key) const {
    if (headPtr == nullptr) return false;
    Node* p = headPtr;

    while (p != nullptr) {
        if (p->m_key == key) return true;
        p = p->nextNode;
    }
    return false;
}