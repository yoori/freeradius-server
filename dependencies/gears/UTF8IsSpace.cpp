#include <gears/UTF8IsProperty.hpp>

namespace Gears
{
  namespace UnicodeProperty
  {
    static TreeNode NODE_E1 =
    {
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000001ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull,
    };

    static TreeNode NODE_E2 =
    {
      0x00008000000007FFull, 0x0000000080000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull,
    };

    static TreeNode NODE_E3 =
    {
      0x0000000000000001ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull,
    };

    TreeStartNode SPACE_TREE =
    {
      0, 0, 0, 0, 0, 0, 0, 0, 0, &TREE_STOP, &TREE_STOP, &TREE_STOP,
      &TREE_STOP, &TREE_STOP, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, &TREE_STOP, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0x0000000100000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
      0x0000000000000000ull, 0, NODE_E1, NODE_E2, NODE_E3, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };
  } // namespace UnicodeProperty
} // namespace String
