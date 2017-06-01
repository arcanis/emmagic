#include <array>
#include <cstdint>
#include <list>
#include <map>
#include <set>
#include <vector>

class Custom {

 public:

    Custom(std::vector<int> const & data) : m_data(data)
    {
    }

    std::vector<int> const & toJS(void) const
    {
        return m_data;
    }

    static Custom fromJS(std::vector<int> const & vector)
    {
        return Custom(vector);
    }

 private:

    std::vector<int> m_data;

};

Custom testCustom(Custom data)
{
    return data;
}

std::array<std::uint8_t, 3> testArray(std::array<std::uint8_t, 3> array)
{
    return array;
}

std::list<std::uint8_t> testList(std::list<std::uint8_t> list)
{
    return list;
}

std::map<std::uint8_t, std::uint8_t> testMap(std::map<std::uint8_t, std::uint8_t> map)
{
    return map;
}

std::set<std::uint8_t> testSet(std::set<std::uint8_t> set)
{
    return set;
}

std::vector<std::uint8_t> testVector(std::vector<std::uint8_t> vector)
{
    return vector;
}

#include <emscripten/bind.h>
#include <emmagic/magic.hh>
#include <emmagic/stl.hh>
#include <iostream>
EMSCRIPTEN_BINDINGS(test)
{
    using namespace emmagic;

    std::cout << emmagic_has_tojs<Custom>::value << std::endl;

    function<testArray>("testArray");
    function<testVector>("testVector");

    function<testArray, EMMAGIC_USE_TYPED_ARRAYS>("testTypedArray");
    function<testVector, EMMAGIC_USE_TYPED_ARRAYS>("testTypedVector");

    function<testSet>("testSet");
    function<testMap, EMMAGIC_COERCE_KEYS>("testMap");

    function<testSet, EMMAGIC_USE_REAL_SETS>("testRealSet");
    function<testMap, EMMAGIC_USE_REAL_MAPS | EMMAGIC_COERCE_KEYS>("testRealMap");

    function<testCustom>("testCustom");
}
