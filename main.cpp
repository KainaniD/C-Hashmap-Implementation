#include "Map.h"
#include <cassert>
#include <iostream>

int main()
{
	// test cases, assuming KeyType is std::string and ValueType is double
	KeyType key1 = "donna";
	ValueType val1 = 9;
	KeyType key2 = "wu";
	ValueType val2 = 14;

	// testing on an empty Map
	Map m;
	assert(m.size() == 0); // test size
	assert(m.empty()); // test empty
	assert(!m.update(key1, val1)); // test update
	assert(!m.erase(key1)); // test erase
	assert(!m.contains(key1)); // test contains
	assert(!m.get(key1, val1) && val1 == 0); // test get with 2 params
	assert(!m.get(0, key1, val1) && key1 == "donna" && val1 == 9); // test get with 3 params

	// testing on a Map with 1 Node
	assert(m.insert(k, v)); // test insert
	assert(!m.insert(k, v));
	assert(m.contains(k) && m.size() == 1 && !m.empty() && m.get(k, d_Val) && d_Val == 1.0);
	assert(m.update(k, 2.5)); // test update
	assert(!m.update("random", 0));
	assert(m.size() == 1 && !m.empty() && m.get(k, d_Val) && d_Val == 2.5);

	d_Val = 0;
	assert(m.get(0, d_Key, d_Val) && d_Val == 2.5); // testing get with 3 params
	assert(m.erase(k)); // testing erase, m is now empty
	assert(m.size() == 0);
	assert(m.empty());


	// testing on a Map with more than 1 Node
	m.insert("a", 1.0);
	m.insert("b", 2.0);
	m.insert("c", 3.0); // now m has 3 Nodes
	assert(m.size() == 3 && m.contains("a") && m.contains("b") && m.contains("c"));
	assert(m.get("c", d_Val) && d_Val == 3.0); // test get with 2 params
	assert(!m.get("random", d_Val) && d_Val == 3.0);
	assert(m.get(1, d_Key, d_Val) && d_Key == "b" && d_Val == 2.0); // test get with 3 params
	assert(!m.get(-1, d_Key, d_Val) && d_Key == "b" && d_Val == 2.0);
	assert(m.insertOrUpdate("a", 4.0) && m.size() == 3 && m.get("a", d_Val) && d_Val == 4.0); // test insertOrUpdate with update
	assert(m.insertOrUpdate("d", 5.0) && m.size() == 4 && m.contains("d") && m.get("d", d_Val) && d_Val == 5.0); // test insertOrUpdate with insertion
	assert(m.erase("a") && m.erase("b")); // test erase
	assert(m.size() == 2 && m.contains("c") && m.contains("d")); // m now only contains 2 nodes

	// testing copy constructor
	Map empty;
	Map d1 = empty; // test copy empty Map
	assert(d1.empty() && d1.size() == 0);
	Map d2 = m; // test copy non-empty Map
	assert(d2.size() == 2);
	assert(d2.contains("c"));
	assert(d2.contains("d"));
	m.erase("c");
	m.update("d", 9.0);
	assert(d2.contains("c") && d2.get("d", d_Val) && d_Val == 5.0); // test that Nodes of d2 are not dependent on Nodes of m

	// testing assignment operator
	d2 = empty; // test assign empty Map
	assert(d2.size() == 0 && d2.empty());
	d1 = m; // test assign non-empty Map
	assert(d1.size() == 1 && d1.contains("d") && d1.get("d", d_Val) && d_Val == 9.0);
	m.erase("d");
	m.insert("e", 6.0); // m now only contains e
	assert(d1.size() == 1 && d1.contains("d") && d1.get("d", d_Val) && d_Val == 9.0); // test that Nodes of d1 are not dependent on Nodes of m
	m = m; // test assign Map to the same Map
	assert(!m.empty() && m.size() == 1 && m.contains("e"));

	m.insert("f", 7.0); // m now contains 2 Nodes

	// testing swap
	Map d3;
	empty.swap(d3); // test swap empty with empty
	assert(empty.empty() && d3.empty());
	d3.swap(m); // test swap empty with non-empty
	assert(d3.size() == 2 && m.empty() && d3.contains("e") && d3.contains("f"));
	assert(d3.get("e", d_Val) && d_Val == 6.0);
	assert(d3.get("f", d_Val) && d_Val == 7.0);
	d3.swap(d1); // test swap non-empty with non-empty
	assert(d3.size() == 1 && d1.size() == 2 && d3.contains("d") && d1.contains("e") && d1.contains("f"));
	assert(d3.get("d", d_Val) && d_Val == 9.0);
	assert(d1.get("e", d_Val) && d_Val == 6.0);
	assert(d1.get("f", d_Val) && d_Val == 7.0);
	d1.swap(d1); // test swap on same map
	assert(d1.size() == 2);
	assert(d1.get("e", d_Val) && d_Val == 6.0);
	assert(d1.get("f", d_Val) && d_Val == 7.0);

	// testing merge
	Map m1, m2, m3, m4, t1, t2, t3;
	m1.insert("a", 1.0);
	m1.insert("b", 2.0);
	m1.insert("c", 3.0);
	m1.insert("d", 4.0);
	m2.insert("e", 5.0);
	m2.insert("f", 6.0);
	m2.insert("c", 3.0);
	m2.insert("d", 4.0);
	m3.insert(k, v);
	t1 = m4;
	t2 = m4;
	// no mismatched values
	assert(merge(t1, t2, t3) && t3.empty()); // test merge all empty
	t2 = m3;
	assert(merge(t1, t2, t3) && t3.size() == 1 && t3.contains(k)); // test merge 1 empty, 1 non-empty with an empty
	t1 = m1;
	t2 = m2;
	assert(merge(t1, t2, t3) && t3.size() == 6); // test merge 2 non-empty with a non-empty
	assert(t3.contains("a") && t3.contains("b") && t3.contains("c") && t3.contains("d") && t3.contains("e") && t3.contains("f"));
	assert(merge(t1, t2, t1) && t1.size() == 6); // with the same map
	t1 = m1;
	assert(merge(t1, t2, t2) && t2.size() == 6);
	// mismatched values
	t3 = m4;
	m2.update("d", 9.0);
	t2 = m2;
	assert(!merge(t1, t2, t3) && t3.size() == 5 && !t3.contains("d")); // normal case
	assert(!merge(t1, t2, t1) && t1.size() == 5 && !t1.contains("d")); // with the same map
	t1 = m1;
	assert(!merge(t1, t2, t2));
	assert(t2.size() == 5);
	assert(!t2.contains("d"));

	// testing reassign
	Map result;
	reassign(m4, result); // test reassign empty with empty
	assert(result.empty());
	reassign(m3, result); // test reassign non-empty size 1 with empty
	assert(result.size() == 1 && result.contains(k) && result.get(k, d_Val) && d_Val == v);
	reassign(m1, result); // test reassign non-empty size>1 with non-empty
	assert(result.size() == m1.size());
	assert(result.get("a", d_Val) && d_Val != 1.0);
	assert(result.get("b", d_Val) && d_Val != 2.0);
	assert(result.get("c", d_Val) && d_Val != 3.0);
	assert(result.get("d", d_Val) && d_Val != 4.0);
	m1.update("d", 1.0);
	reassign(m1, result); // test reassign non-empty size>1 with non-empty, with duplicate values
	assert(result.size() == m1.size());
	assert(result.get("a", d_Val) && d_Val == 2.0);
	assert(result.get("b", d_Val) && d_Val == 3.0);
	assert(result.get("c", d_Val) && d_Val == 1.0);
	assert(result.get("d", d_Val) && d_Val == 1.0);
	reassign(m1, m1); // test reassign with the same Map
	assert(m1.get("a", d_Val) && d_Val == 2.0);
	assert(m1.get("b", d_Val) && d_Val == 3.0);
	assert(m1.get("c", d_Val) && d_Val == 1.0);
	assert(m1.get("d", d_Val) && d_Val == 1.0);
	reassign(m4, m4); // test reassign with the same Map, empty
	assert(m4.empty());

	std::cout << "All tests passed." << std::endl;
}