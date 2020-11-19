This is a program that just gets posts from reddit. I might add a bunch more features later.
Dependencies:
a standard libc,
libcurl,
a C compiler.

# Usage
To install this run:
```make```
This will create a binary, put it where ever you want.

This program has 4 required args, subreddit, sort, and range.
any subreddit on reddit is valid for the subreddit arg, 
valid args for sort are: 
```tt(top today), tn(top now), tw(top this week), tm(top this month), ty(top this year), ta(top all time), h(hot), and n(new)```
and range is just a range of 2 numbers. 
This example gets the top 5 posts from r/copypasta top all time:
```reddit-get copypasta ta 0 5```
