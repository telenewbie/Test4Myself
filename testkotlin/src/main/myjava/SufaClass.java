package myjava;

import javax.swing.tree.TreeNode;
import java.util.*;

public class SufaClass {
    static class Solution {
        public List<List<Integer>> permute(int[] nums) {
            // 回溯算法
            // 假定第一个可以 在遍历其他，依次遍历下去，如果不行就回溯继续，
            //如果成功解，则需要过滤该解，直到所有解都成功下去
            List<List<Integer>> result = new ArrayList<>();
            List<Integer> num_lists = new ArrayList<>();
            // 转换成 list
            for (int num : nums) {
                num_lists.add(num);
            }
            backTrace(num_lists.size(), 0, num_lists, result);

            return result;
        }

        public void backTrace(int size, int start, List<Integer> nums, List<List<Integer>> result) {
            if (size == start) // 结束的情况下？
                result.add(new ArrayList<>(nums));
            //start 和其他值进行交换
            for (int i = start; i < size; i++) {
                Collections.swap(nums, start, i);
                backTrace(size, start + 1, nums, result);
                Collections.swap(nums, start, i);
            }
        }

        //        public int lengthOfLongestSubstring(String s) {
//            int n = s.length();
//            Set<Character> set = new HashSet<>();
//            int ans = 0, i = 0, j = 0;
//            while (i < n && j < n) {
//                // try to extend the range [i, j]
//                if (!set.contains(s.charAt(j))) {
//                    set.add(s.charAt(j++));
//                    ans = Math.max(ans, j - i);
//                } else {
//                    set.remove(s.charAt(i++));
//                }
//                System.out.println("i=" + i + ",j=" + j + ",set=" + set);
//            }
//            return ans;
//        }
        public int lengthOfLongestSubstring(String s) {
            int n = s.length(), ans = 0;
            Map<Character, Integer> map = new HashMap<>(); // current index of character
            // try to extend the range [i, j]
            for (int j = 0, i = 0; j < n; j++) {
                if (map.containsKey(s.charAt(j))) {
                    i = Math.max(map.get(s.charAt(j)), i);
                }
                ans = Math.max(ans, j - i + 1);
                map.put(s.charAt(j), j + 1);
                System.out.println("i=" + i + ",j=" + j + ",set=" + map);
            }
            return ans;
        }

        public static class ListNode {
            int val;
            ListNode next;

            ListNode(int x) {
                val = x;
            }
        }

        public ListNode mergeTwoLists(ListNode l1, ListNode l2) {
            // maintain an unchanging reference to node ahead of the return node.
            ListNode prehead = new ListNode(-1);

            ListNode prev = prehead;
            while (l1 != null && l2 != null) {
                if (l1.val <= l2.val) {
                    prev.next = l1;
                    l1 = l1.next;
                } else {
                    prev.next = l2;
                    l2 = l2.next;
                }
                prev = prev.next;
            }

            // exactly one of l1 and l2 can be non-null at this point, so connect
            // the non-null list to the end of the merged list.
            prev.next = l1 == null ? l2 : l1;

            return prehead.next;
        }


        public List<List<Integer>> threeSum(int[] nums) {
            Arrays.sort(nums);
            List<List<Integer>> result = new ArrayList<>();
            int size = nums.length;
            int L= 0 ;  // 左点
            int R = 0;  // 右点
            for(int i = 0; i < size; i++){
                // 如果第一个数值 大于0  的话，则结果一定大于0 ，因为已经排序好了
                if(nums[i]>0) return result;
                if(i>0&&nums[i] == nums[i-1]) continue;
                L  = i+1; // 从当前的下一个开始
                R  = size -1; // 从末尾开始
                while(L<R){
                    int target =0- (nums[i] + nums[L]);

                    if(target>nums[R])  L++;
                    else if(target <nums[R]) R--;
                    else {
                        // 相等的情况
                        List<Integer> r  = new ArrayList<>();
                        r.add(nums[i]);
                        r.add(nums[L]);
                        r.add(nums[R]);
                        result.add(r);
                        while(L<R&&nums[L] == nums[L+1]) L++;
                        while(L<R&&nums[R] == nums[R-1]) R--;
                        L++;
                        R--;
                    }

                }

            }
            return result;
        }
    }

    static class LRUCache extends LinkedHashMap<Integer,Integer>{

        private int mCapacity;
        public LRUCache(int capacity) {
            super(capacity, 0.75F,true);
            mCapacity= capacity;
        }

        public int get(int key) {
            return super.getOrDefault(key, -1);
        }

        public void put(int key, int value) {
            super.put(key,value);
        }

        @Override
        public boolean removeEldestEntry(Map.Entry<Integer,Integer> eldest){
            return size()>mCapacity;
        }
    }

    public static void main(String[] args) {
        int[] nums = new int[]{1, 2, 3};
        Solution solution = new Solution();
        List<List<Integer>> permute = solution.permute(nums);
        System.out.println(permute);
//        int abcbab = solution.lengthOfLongestSubstring("abcbaabb");
//        System.out.println(abcbab);
        Solution.ListNode node1 = new Solution.ListNode(1);
        Solution.ListNode node2 = new Solution.ListNode(2);

        System.out.println(solution.mergeTwoLists(node1, node2));
        LRUCache cache = new LRUCache(2);
        cache.put(1, 1);
        cache.put(2, 2);
        System.out.println(cache.get(1));       // 返回  1
        cache.put(3, 3);    // 该操作会使得密钥 2 作废
        System.out.println(cache.get(2));       // 返回 -1 (未找到)
        cache.put(4, 4);    // 该操作会使得密钥 1 作废
        System.out.println(cache.get(1));       // 返回 -1 (未找到)
        System.out.println(cache.get(3));       // 返回  3
        System.out.println(cache.get(4));       // 返回  4

        System.out.println(cache);
        List<List<Integer>> result = new ArrayList<>();
        List<Integer> r = new ArrayList<>();
        r.add(3);
        r.add(2);
        r.add(1);
        result.add(r);
        List<Integer> r1 = new ArrayList<>();
        r1.add(1);
        r1.add(2);
        r1.add(3);
        Collections.sort(r);
        if(!result.contains(r))
            result.add(r);
        System.out.println(result);
    }
}
