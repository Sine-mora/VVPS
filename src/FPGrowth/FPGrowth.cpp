#include <algorithm>
#include <cassert>
#include <utility>

#include <FPGrowth/FPGrowth.hpp>
#include <iostream>

#define TREE_CLASS "FP::Tree"
namespace FP
{
Node::Node(const Item& item, const SharedNode& ptrParent)
    : m_item(item),
      m_unFrequency(1),
      m_ptrNodeLink(nullptr),
      m_ptrParent(ptrParent),
      m_vecChildren()
{
}


Tree::Tree(const DataBase& dataBase, uint64_t unMinSupportedThreshold)
    : m_ptrRoot(std::make_shared<Node>(Item{}, nullptr)),
      m_mapNodesLinkedToItem{},
      m_unMinSupportedThreshold(unMinSupportedThreshold)
{
    /** Order the items in the data base by their frequency. */
    const auto& setOrderedItems = OrderItemsByFrequency(dataBase, unMinSupportedThreshold);

    /** Start tree construction */
    ConstructTreeNodes(dataBase, setOrderedItems);
}

bool Tree::IsEmpty() const {
    assert(m_ptrRoot);
    return m_ptrRoot->m_vecChildren.size() == 0;
}

SetPairFrequencyItem Tree::OrderItemsByFrequency(const DataBase& dataBase,
                                             uint64_t unMinSupportedThreshold)
{
    /**
     * @brief mapFrequencyByItem - holds each item in the transactions vector
     * and how many time it's found (that's its frequency).
     */
    std::map<Item, uint64_t> mapFrequencyByItem;

    /** Scan the transactions counting the frequence of each item. */
    for(const Transaction& transaction : dataBase)
    {
        /** Scan the frequency of every item in the current transaction. */
        for(const Item& item : transaction)
        {
            /** Increment the frequency of the current transaction. */
            ++mapFrequencyByItem[item];
        }
    }

    /** Keep only items which have a frequency greater or equal than
     * the minimum support treshold. */
    for(auto it = mapFrequencyByItem.cbegin(); it != mapFrequencyByItem.cend();)
    {
        /** Get the frequency of the current item. */
        const uint64_t unItemFrequency = (*it).second;
        /** Check if the doesn't exceed the minimum required treshold. */
        if(unItemFrequency < unMinSupportedThreshold)
        {
            /** Erase the current item from the map, because it doesn't meet
             * the minimum required threshold. */
            mapFrequencyByItem.erase(it++);
        }
        else
        {
            /** Continue iterating to the next item in the map. */
            ++it;
        }
    }

    /** Order items by decreasing frequency. */

    /**
     * @brief setItemsOrderedByFrequency - contains a sorted set of unique items
     * from the transactions, in decreasing oreder by their frequency.
     */
    SetPairFrequencyItem setItemsOrderedByFrequency;

    /** Iterate through each item-frequency pair */
    for(const auto& pair : mapFrequencyByItem)
    {
        /** Get the item and its frequency from the current pair. */
        const Item& item = pair.first;
        const uint64_t frequency = pair.second;

        /** Insert a PairFrequencyItem object in the set. */
        setItemsOrderedByFrequency.insert({ frequency, item });
    }

    return setItemsOrderedByFrequency;
}

void Tree::ConstructTreeNodes(const DataBase& dataBase,
                              const SetPairFrequencyItem& setOrderedItems)
{
    /** Scan the transactions again. */
    uint64_t unCurrTransaction = 0;
    for(const Transaction& transaction : dataBase)
    {
        /** Select and sort the frequent items in transaction,
         *  according to the order of setOrderedItems */
        SelectAndSortTransactionItems(transaction, setOrderedItems);
    }
}

void Tree::SelectAndSortTransactionItems(const Transaction& transaction, const SetPairFrequencyItem& setOrderedItems)
{
    /**
     * @brief ptrCurrNode - set the current node to the root before
     * scanning the current transaction.
     */
    SharedNode ptrCurrNode = m_ptrRoot;
    for(const auto& pair : setOrderedItems)
    {
        /**
         * @brief currItem - Retrieve the current item from the ordered set.
         */
        const Item& currItem = pair.second;

        /** Check if item is contained in the current transaction. */
        bool isItemFound = std::find(transaction.cbegin(), transaction.cend(),
                                     currItem) != transaction.cend();
        if(isItemFound)
        {
            /** Insert item in the tree.*/

            /** Check if ptrCurrNode has a child ptrCurrNodeChild,
             * such that ptrCurrNodeChild.item = strCurrItem. */
            const auto nodeIt = std::find_if(ptrCurrNode->m_vecChildren.cbegin(),
                                         ptrCurrNode->m_vecChildren.cend(),
                                         [currItem](const SharedNode& ptrNode)
            {
                return ptrNode->m_item == currItem;
            });

            /** Check if the child exists. */
            if(nodeIt == ptrCurrNode->m_vecChildren.cend())
            {
                /** The child doesn't exist, create a new node */
                /** Advance to the next node of the current transaction. */
                ptrCurrNode = CreateChildNode(ptrCurrNode, currItem);
            }
            else
            {
                /**
                 * @brief ptrCurrNodeChild - the child exists, increment
                 * its frequency.
                 */
                SharedNode ptrCurrNodeChild = *nodeIt;
                ++ptrCurrNodeChild->m_unFrequency;

                /** Advance to the next node of the current transaction */
                ptrCurrNode = ptrCurrNodeChild;
            }
        }
    }
}

SharedNode Tree::CreateChildNode(SharedNode ptrNodeParent, const Item& item)
{
    assert(ptrNodeParent);
    assert(!item.empty());

    /** Create a new child node. */
    const SharedNode ptrNodeChild = std::make_shared<Node>(item, ptrNodeParent);

    /** Add the new node to the tree */
    ptrNodeParent->m_vecChildren.push_back(ptrNodeChild);

    /** Update the node-link structure. */
    if(m_mapNodesLinkedToItem.count(ptrNodeChild->m_item))
    {
        SharedNode ptrPrevNode = m_mapNodesLinkedToItem[ptrNodeChild->m_item];
        while(ptrPrevNode->m_ptrNodeLink)
        {
            ptrPrevNode = ptrPrevNode->m_ptrNodeLink;
        }
        ptrPrevNode->m_ptrNodeLink = ptrNodeChild;
    }
    else
    {
        m_mapNodesLinkedToItem[ptrNodeChild->m_item] = ptrNodeChild;
    }

    return ptrNodeChild;
}


PatternSet TreeGrowth::ConstructPatternSetFromTree(const Tree &tree)
{
    if(tree.IsEmpty())
    {
        return PatternSet{};
    }

    if(TreeGrowth::ContainsSinglePath(tree))
    {
        /* Generate all possible combinations of the items in the tree. */
        return ConstructSinglePathPatternSet(tree);
    }
    else
    {
        /** Generate conditional fptrees for each different item in the tree,
         * then join the results */
        return ConstructMultiPathPatternSet(tree);
    }
}

bool TreeGrowth::ContainsSinglePath(const SharedNode& ptrNode)
{
    assert(ptrNode);
    if(ptrNode->m_vecChildren.size() == 0)
    {
        return true;
    }
    if(ptrNode->m_vecChildren.size() > 1)
    {
        return false;
    }
    return TreeGrowth::ContainsSinglePath(ptrNode->m_vecChildren.front());
}

bool TreeGrowth::ContainsSinglePath(const Tree& tree)
{
    if (tree.IsEmpty())
    {
        return true;
    }
    return TreeGrowth::ContainsSinglePath(tree.m_ptrRoot);
}

PatternSet TreeGrowth::ConstructSinglePathPatternSet(const Tree& tree)
{
    PatternSet singlePathPatternSet;

    /* For each node in the tree. */
    assert(tree.m_ptrRoot->m_vecChildren.size() == 1);
    SharedNode ptrCurrNode = tree.m_ptrRoot->m_vecChildren.front();
    while(ptrCurrNode)
    {
        const Item& strCurrTreeNodeItem = ptrCurrNode->m_item;
        const uint64_t unCurrTreeNodeFrequency = ptrCurrNode->m_unFrequency;

        /** Add a pattern, formed only by the item of the current node. */
        Pattern newPattern = { { strCurrTreeNodeItem }, unCurrTreeNodeFrequency };
        singlePathPatternSet.insert(newPattern);

        // create a new pattern by adding the item of the current node to each pattern generated until now
        for(const Pattern& pattern : singlePathPatternSet)
        {
            Pattern new_pattern{ pattern };
            new_pattern.first.insert(strCurrTreeNodeItem);
            assert(unCurrTreeNodeFrequency <= pattern.second);
            new_pattern.second = unCurrTreeNodeFrequency;

            singlePathPatternSet.insert(new_pattern);
        }

        /** Advance to the next node, until the end of the tree. */
        assert(ptrCurrNode->m_vecChildren.size() <= 1);
        if(ptrCurrNode->m_vecChildren.size() == 1)
        {
            ptrCurrNode = ptrCurrNode->m_vecChildren.front();
        }
        else
        {
            ptrCurrNode = nullptr;
        }
    }

    return singlePathPatternSet;
}

PatternSet TreeGrowth::ConstructMultiPathPatternSet(const Tree& tree)
{
    PatternSet multiPathPatternSet;

    /** For each item in the tree. */
    for(const auto& pair : tree.m_mapNodesLinkedToItem)
    {
        const auto& currItem = pair.first;
        const auto& ptrCurrNodeStartingPath = pair.second;

        /** Build the conditional tree, relative to the current item. */

        /** Start by generating the conditional pattern base. */
        const auto& conditionalPatternBase = TreeGrowth::ConstructConditionalPatternBase(ptrCurrNodeStartingPath);

        /** Generate the transactions that represent the conditional pattern base. */
        const auto& conditionalTreeDataBase = TreeGrowth::ConstructConditionalTreeDataBase(conditionalPatternBase);

        /** Build the conditional tree, relative to the current item with
         * the transactions just generated */
        const Tree conditionalTree(conditionalTreeDataBase,
                                   tree.m_unMinSupportedThreshold);

        /** Call recursively ConstructPatternSetFromTree on the conditional tree
         * (empty tree: no patterns) */
        const auto& setConditionalPatterns =
                TreeGrowth::ConstructPatternSetFromTree(conditionalTree);

        /** Construct patterns, relative to the current item, using both
         * the current item and the conditional patterns. */
        const auto& setCurrItemPatterns = TreeGrowth::ConstructItemPatternSet(ptrCurrNodeStartingPath,
                                                                                    currItem,
                                                                                    setConditionalPatterns);

        /* Join the patterns generated by the current item
         * with all the other items of the tree. */
        multiPathPatternSet.insert(setCurrItemPatterns.cbegin(),
                                    setCurrItemPatterns.cend());
    }

    return multiPathPatternSet;
}

VecTransformedPrefixPaths TreeGrowth::ConstructConditionalPatternBase(SharedNode ptrNodeStartingPath)
{
    /** Start by generating the conditional pattern base. */
    std::vector<TransformedPrefixPath> vecConditionalPatternBase;

    /** For each path in the header_table (relative to the current item). */
    while(ptrNodeStartingPath)
    {
        /** Construct the transformed prefix path */

        /** each item in th transformed prefix path has the same frequency (the frequency of ptrNodeStartingPath). */
        const uint64_t unStartingNodeFrequency = ptrNodeStartingPath->m_unFrequency;

        SharedNode ptrNodeCurrPath = ptrNodeStartingPath->m_ptrParent.lock();
        /** Check if curr_path_fpnode is already the root of the tree. */
        if(ptrNodeCurrPath->m_ptrParent.lock())
        {
            /** The path has at least one node (excluding the starting node and the root) .*/
            TransformedPrefixPath transformedPrefixPath{ {}, unStartingNodeFrequency };

            while(ptrNodeCurrPath->m_ptrParent.lock())
            {
                /** Assert that the frequency is correct. */
                assert(ptrNodeCurrPath->m_unFrequency >= unStartingNodeFrequency);
                transformedPrefixPath.first.push_back(ptrNodeCurrPath->m_item);

                /** Advance to the next node in the path. */
                ptrNodeCurrPath = ptrNodeCurrPath->m_ptrParent.lock();
            }

            vecConditionalPatternBase.push_back(transformedPrefixPath);
        }

        /** Advance to the next path. */
        ptrNodeStartingPath = ptrNodeStartingPath->m_ptrNodeLink;
    }
    return vecConditionalPatternBase;
}

DataBase TreeGrowth::ConstructConditionalTreeDataBase(const VecTransformedPrefixPaths& vecConditionalPatternBase)
{
    DataBase conditionalTreeDataBase;
    for(const TransformedPrefixPath& transformedPrefixPath : vecConditionalPatternBase)
    {
        const std::vector<Item>& vecTransformedPrefixPathItems = transformedPrefixPath.first;
        const uint64_t unTransformedPrefixPathItemFrequency = transformedPrefixPath.second;

        Transaction transaction;
        for(const Item& item : vecTransformedPrefixPathItems)
        {
            transaction.push_back(item);
        }

        /** Add the same transaction unTransformedPrefixPathItemFrequency times */
        for(uint64_t i = 0; i < unTransformedPrefixPathItemFrequency; ++i)
        {
            conditionalTreeDataBase.push_back(transaction);
        }
    }
    return conditionalTreeDataBase;
}

PatternSet TreeGrowth::ConstructItemPatternSet(SharedNode ptrStartingPathNode, const Item& item,
                                                    const PatternSet& conditionalPatternSet)
{
    /** Construct patterns, relative to the current item, using both
     * the current item and the conditional patterns. */
    PatternSet setCurrItemPatterns;

    /** The first pattern is made only by the current item. */
    /** Compute the frequency of this pattern by summing the frequency
     *  of the nodes which have the same item (follow the node links) */
    uint64_t unCurrItemFrequency = 0;
    while(ptrStartingPathNode)
    {
        unCurrItemFrequency += ptrStartingPathNode->m_unFrequency;
        ptrStartingPathNode = ptrStartingPathNode->m_ptrNodeLink;
    }

    /** Add the pattern as a result. */
    Pattern pattern{ { item }, unCurrItemFrequency };
    setCurrItemPatterns.insert(pattern);

    /* The next patterns are generated by adding the current item
     * to each conditional pattern. */
    for(const Pattern& pattern : conditionalPatternSet)
    {
        Pattern newPattern{ pattern };
        newPattern.first.insert(item);
        assert(unCurrItemFrequency >= pattern.second);
        newPattern.second = pattern.second;

        setCurrItemPatterns.insert({ newPattern });
    }
    return setCurrItemPatterns;
}

void PrintPatternSet(const PatternSet& patternSet)
{
    std::cout << "This pattern set contains " << patternSet.size() << " patterns.\n";
    uint64_t unCurrPattern = 0;
    for(const auto& pattern: patternSet)
    {
        std::cout << "Pattern[" << (unCurrPattern++) << "]:{\n{ ";
        const auto& items = pattern.first;
        const auto& frequency = pattern.second;
        for(const auto& item: items)
        {
            std::cout << "\"" << item << "\" ";
        }
        std::cout << "}\nFrequency: " << frequency << " }\n\n";
    }
}

void PrintTransaction(const Transaction& transaction)
{
    std::cout << "Transaction { ";
    for(const auto& item: transaction)
    {
        std::cout << "\"" << item << "\" ";
    }
    std::cout << " }\n";
}

}
