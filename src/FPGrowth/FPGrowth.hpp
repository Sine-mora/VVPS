#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <utility>

/**
 * @namespace FP - Frequent Pattern Growth Algorithm which represents
 * the database in the form an FP Tree. A frequent pattern is generated without
 * the need for candidate generation. FP growth algorithm represents the database
 * in the form of a tree called a frequent pattern tree or FP tree.
 *
 *
 * This tree structure will maintain the association between the itemsets.
 * The database is fragmented using one frequent item. This fragmented part
 * is called “pattern fragment”. The itemsets of these fragmented patterns are
 * analyzed. Thus with this method, the search for frequent itemsets is reduced
 * comparatively.
 *
 * Algorithm steps:
 * #1) The first step is to scan the database to find the occurrences of the
 * itemsets in the database. The count of 1-itemsets in the database is called
 * support count or frequency of 1-itemset.
 *
 * #2) The second step is to construct the FP tree. For this, create the root
 * of the tree. The root is represented by null.
 *
 * #3) The next step is to scan the database again and examine the transactions.
 * Examine the first transaction and find out the itemset in it. The itemset with
 * the max count is taken at the top, the next itemset with lower count and so on.
 * It means that the branch of the tree is constructed with transaction itemsets
 * in descending order of count.
 *
 * #4) The next transaction in the database is examined. The itemsets are ordered
 * in descending order of count. If any itemset of this transaction is already present
 * in another branch (for example in the 1st transaction), then this transaction
 * branch would share a common prefix to the root.
 *
 * This means that the common itemset is linked to the new node of another
 * itemset in this transaction.
 *
 * #5) Also, the count of the itemset is incremented as it occurs in the transactions.
 * Both the common node and new node count is increased by 1 as they are created
 * and linked according to transactions.
 *
 * #6) The next step is to mine the created FP Tree. For this, the lowest node
 * is examined first along with the links of the lowest nodes. The lowest node
 * represents the frequency pattern length 1. From this, traverse the path in
 * the FP Tree. This path or paths are called a conditional pattern base.
 *
 * Conditional pattern base is a sub-database consisting of prefix paths in the
 * FP tree occurring with the lowest node (suffix).
 *
 * #7) Construct a Conditional FP Tree, which is formed by a count of itemsets
 * in the path. The itemsets meeting the threshold support are considered in the
 * Conditional FP Tree.
 *
 * #8) Frequent Patterns are generated from the Conditional FP Tree.
 */
namespace FP
{

/** Forward declarations */
struct Node;
struct Tree;

/** Represents an item. */
using Item = std::string;

/** Represents a list of items. */
using Transaction = std::vector<Item>;

/** Represents a list of transactions. */
using DataBase = std::vector<Transaction>;

/** first - vector of items, second - their frequency. */
using TransformedPrefixPath = std::pair<std::vector<Item>, uint64_t>;

/** Represents a list of transformed prefix paths. */
using VecTransformedPrefixPaths = std::vector<TransformedPrefixPath>;

/** first - an item pattern, second - its frequency. */
using Pattern = std::pair<std::set<Item>, uint64_t>;

/** Represents a sorted set of unique patterns. */
using PatternSet = std::set<Pattern>;

/** Represents a tree node, which has multiple owners. */
using SharedNode = std::shared_ptr<Node>;

/** Represents a tree node, which doesn't have an owner. */
using WeakNode = std::weak_ptr<Node>;

/** Represents a list of shared tree nodes. */
using VecSharedNodes = std::vector<SharedNode>;

/** first - frequency of the item, second - item. */
using PairFrequencyItem = std::pair<uint64_t, Item>;

/**
 * @brief The SDecreasingOrderComparator struct - used as a comparator class
 * for comparing two items, based on their frequency.
 */
struct SDecreasingOrderComparator
{
    /**
     * @brief operator () - overload the bool operator and use it to
     * compare two items by their frequency.
     * @param lhs - left hand side of the comparison.
     * @param rhs - right hand side of the comparison.
     * @return true if lsh has a greater frequency than rhs, or if that's
     * not the case if lsh is above rhs in terms of dictionary order.
     */
    bool operator() (const PairFrequencyItem& lhs, const PairFrequencyItem& rhs) const
    {
        return std::tie(lhs) < std::tie(rhs);
    }
};

/** Represents a sorted set of unique items in decreasing oreder by their frequency. */
using SetPairFrequencyItem = std::set<PairFrequencyItem, SDecreasingOrderComparator>;

/**
 * @brief The Node struct
 */
struct Node
{
    /**
     * @brief m_item - the data item of this node.
     */
    const Item m_item;

    /**
     * @brief m_unFrequency - frequency of the item, how many times it occurs
     * in the data set.
     */
    uint64_t m_unFrequency;

    /**
     * @brief m_ptrNodeLink - a pointer to this node's sibling.
     */
    SharedNode m_ptrNodeLink;

    /**
     * @brief m_ptrParent - a pointer to this node's parent.
     */
    WeakNode m_ptrParent;

    /**
     * @brief m_vecChildren - a list of this node's children.
     */
    VecSharedNodes m_vecChildren;

    /**
     * @brief Node - default constructor for this tree node.
     * @param item - the item which this node will contain.
     * @param ptrParent - pointer to the parent of this node.
     */
    Node(const Item& item, const SharedNode& ptrParent);
};

/**
 * @brief The Tree struct - Frequent Pattern Tree is a tree-like structure that
 * is made with the initial itemsets of the database. The purpose of the FP tree
 * is to mine the most frequent pattern. Each node of the FP tree represents
 * an item of the itemset.
 *
 * The root node represents null while the lower nodes represent the itemsets.
 * The association of the nodes with the lower nodes that is the itemsets with
 * the other itemsets are maintained while forming the tree.
 */
struct Tree
{
    /**
     * @brief m_ptrRoot - root node of the tree.
     */
    SharedNode m_ptrRoot;

    /**
     * @brief m_mapHeaderTable - maintain the association nodes, referring
     * to the same item.
     */
    std::map<Item, SharedNode> m_mapNodesLinkedToItem;

    /**
     * @brief m_unMinSupportedThreshold - minimum treshold for the frequency
     * of the items in the tree.
     */
    uint64_t m_unMinSupportedThreshold;

    /**
     * @brief Tree - default constructor for the FP tree.
     * @param dataBase - a list of transactions, containing items.
     * Represents the data set.
     * @param unMinSupportedThreshold - a minimum treshold value which the
     * frequency of an item needs to exceed, in order to be considered as part
     * of the tree.
     */
    Tree(const DataBase& dataBase, uint64_t unMinSupportedThreshold);

    /**
     * @brief IsEmpty - check if the tree's root node contains any children.
     * @return - true, if the tree is empty.
     */
    bool IsEmpty() const;

private:
    /**
     * @brief OrderItemsByFrequency - scan the items in transactions database
     * that exceed the minimum supported treshold and order them by their
     * frequency.
     * @param dataBase - holds the transactions (data set) that need to be scanned.
     * @param unMinSupportedThreshold - used as a minimum treshold to discard
     * items with insufficient frequency.
     * @return a set of ordered items, based on their frequency.
     */
    SetPairFrequencyItem OrderItemsByFrequency(const DataBase& dataBase,
                                           uint64_t unMinSupportedThreshold);

    /**
     * @brief ConstructTreeNodes - construct the FP tree with the provided
     * data set and list of items, based on their frequency in descending order.
     * @param dataBase - a list of transactions, containing items.
     * Represents the data set.
     * @param setOrderedItems - a set of ordered items exceeding the minimum
     * suppport treshold, based on their frequency.
     */
    void ConstructTreeNodes(const DataBase& dataBase,
                            const SetPairFrequencyItem& setOrderedItems);

    /**
     * @brief SelectAndSortTransactionItems - select and sort the items in
     * the passed transaction, based on the frequency order of setOrderedItems
     * @param transaction - a list of items.
     * @param setOrderedItems - a set of ordered items exceeding the minimum
     * suppport treshold, based on their frequency.
     */
    void SelectAndSortTransactionItems(const Transaction& transaction,
                                       const SetPairFrequencyItem& setOrderedItems);

    /**
     * @brief CreateChildNode - create a new child node and add it to its
     * parent's list of children. Update the tree's header table.
     * @param ptrNodeParent - parent of the new child node.
     * @param item - item of the new child node.
     * @return the newly constructed child node.
     */
    SharedNode CreateChildNode(SharedNode ptrNodeParent, const Item& item);
};


/**
 * @brief The TreeGrowth struct - used to find frequent item sets (also closed
 * and maximal as well as generators) with the FP-growth algorithm
 * (Frequent Pattern growth [Han et al. 2000]), which represents the transaction
 * database as a prefix tree which is enhanced with links that organize the
 * nodes into lists, referring to the same item. The search is carried out by
 * projecting the prefix tree, working recursively on the result, and pruning
 * the original tree.
 */
struct TreeGrowth
{
public:
    /**
     * @brief ConstructPatternSetFromTree - construct a pattern set from the
     * provided tree.
     * @param tree - the tree to be examined.
     * @return a pattern set from the provided tree.
     */
    static PatternSet ConstructPatternSetFromTree(const Tree& tree);
private:

    /**
     * @brief ContainsSinglePath - check if the given node contains a single path.
     * @param ptrNode - the node to be checked
     * @return true if the node contains a single path.
     */
    static bool ContainsSinglePath(const SharedNode& ptrNode);

    /**
     * @brief ContainsSinglePath - check if the given tree contains a single path.
     * @param tree - the tree to be checked.
     * @return true if the tree contains a single path.
     */
    static bool ContainsSinglePath(const Tree& tree);

    /**
     * @brief ConstructSinglePathPatternSet - construct a pattern set
     * from the provided tree, if it contains a single path.
     * @param tree - the tree to be examined.
     * @return a pattern set from the provided tree.
     */
    static PatternSet ConstructSinglePathPatternSet(const Tree& tree);

    /**
     * @brief ConstructMultiPathPatternSet - construct a pattern set
     * from the provided tree, if it contains multiple paths.
     * @param tree - the tree to be examined.
     * @return a pattern set from the provided tree.
     */
    static PatternSet ConstructMultiPathPatternSet(const Tree& tree);

    /**
     * @brief ConstructConditionalPatternBase - create a conditional pattern base
     * @param ptrNodeStartingPath - starting path node of the item set.
     * @return a conditional pattern base, created from the provided starting path node.
     */
    static VecTransformedPrefixPaths ConstructConditionalPatternBase(SharedNode ptrNodeStartingPath);

    /**
     * @brief ConstructConditionalTreeDataBase - create a list of transactions,
     * based on the provided conditional pattern base.
     * @param vecConditionalPatternBase - a conditional pattern base,
     * created from the provided starting path node.
     * @return a database, constructed from the provided conditional pattern base.
     */
    static DataBase ConstructConditionalTreeDataBase(const VecTransformedPrefixPaths& vecConditionalPatternBase);


    /**
     * @brief ConstructItemPatternSet - create a pattern set for the provided
     * item, based on the specified conditional pattern set.
     * @param ptrStartingPathNode - starting node of the tree nodes, refering
     * to the provided item.
     * @param item - the item for which a pattern set must be constructed.
     * @param conditionalPatternSet - a conditional pattern set.
     * @return a pattern set for the provided item, based on the conditional
     * pattern set.
     */
    static PatternSet ConstructItemPatternSet(SharedNode ptrStartingPathNode,
                                                     const Item& item,
                                                     const PatternSet& conditionalPatternSet);
};

/**
 * @brief PrintPatternSet - log pattern set information to console.
 * @param patternSet - the pattern set to be printed.
 */
void PrintPatternSet(const PatternSet& patternSet);

/**
 * @brief PrintTransaction - log transaction information to the console.
 * @param transaction - the transaction to be printed.
 */
void PrintTransaction(const Transaction& transaction);
}


