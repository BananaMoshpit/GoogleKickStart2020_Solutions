//Following AI.Cash's guide on segmentation trees at https://codeforces.com/blog/entry/18051 as indicated in question's analysis
#include<stdio.h>
#include<stdlib.h>

#define N 1e5;  // limit for array size
int n;  // array size (amount of candies)
int t[400000]; // tree of candy values: (-1)^(i-1) Ai
int mt[400000]; // tree of candy*index values: (-1)^(i-1) Ai*i 

void build() {  // build the tree

  for (int i = n - 1; i > 0; --i){ 
    t[i] = t[i<<1] + t[i<<1|1]; //build candy values tree
    mt[i] = mt[i<<1] + mt[i<<1|1]; //build candies*weight values tree
  }
  //without bitwise operators:
  //for (int i = n - 1; i > 0; --i) t[i] = t[i * 2] + t[i * 2 + 1];
}

void modify(int p, int value) {  // set values at position p

  t[p + n] = value; // updating leaf values
  if(p&1)
    t[p + n] *= -1;

   mt[p + n] = t[p + n] * (p + 1);

  for (p += n; p > 1; p >>= 1){ // updating parent nodes

      t[p>>1] = t[p] + t[p^1]; // t[p / 2] = t[p] + t[p + 1]
      mt[p>>1] = mt[p] + mt[p^1];
  }

}

int rangeSum(int l, int r, int* Tree) {  // sum on interval [l, r)
  int res = 0;

  for (l += n, r += n; l < r; l >>= 1, r >>= 1) { // l /= 2, r /= 2
    if (l&1) res += Tree[l++]; // if l % 2 != 0
    if (r&1) res += Tree[--r];
  }
  return res;
}

int query(int l, int r){
  int i, mt_sum, t_sum;
  i = 1;
  
  mt_sum = rangeSum(l , r + 1, &mt[0]);
  t_sum = rangeSum(l , r + 1, &t[0]);

    if(l&1) // if Ai has odd index, it's query will be negativated
      i *= -1;

  return i * ( mt_sum - (l) * t_sum ); // this is the formula given at analysis, note that l here is (l - 1) at analysis due to line #82
}

int main() {
  int querySum, tests, operations, left, right, i, j;
  char op;


  scanf("%d", &tests);

  for (i = 0; i < tests; i++) {
    querySum = 0;

    scanf("%d %d", &n, &operations);
    for (int i = 0; i < n; ++i) { // Getting candies
      scanf("%d", t + n + i);
      
      if (i&1)
        t[n + i] *= (-1); // Normal tree: At == -1^( i-1 ) * Ai

      mt[n + i] = (i + 1) * t[n + i]; // Multiple values tree: Amt == At * i
    }
    build();

    for (j = 0; j < operations; j++){  // Performing operations
      scanf("%*c%c %d %d", &op, &left, &right);
      left--, right--;

      if (op == 'Q') //Query
        querySum += query(left, right);

      else
        modify(left, right + 1); // Update
   }
   printf("Case #%d: %d\n", i + 1, querySum);
  }

  return 0;
  
}
