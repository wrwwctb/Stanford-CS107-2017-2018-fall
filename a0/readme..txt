File: readme.txt
Assignment: assign0
Author: YOUR NAME HERE
----------------------

----------------------
QUESTION: Describe two of the most useful things you've learned
to do in UNIX. Write one sentence for each. Write as though you're 
trying to sell a classmate on the utility of learning this particular 
command/trick/tip.
ANSWER:


----------------------
QUESTION: Describe two of the most useful things you've learned
to do in your editor of choice (either vim or emacs--you don't need
to write about both vim and emacs). Write one sentence for each useful
thing you learned. Write as though you're trying to sell a classmate 
on the utility of learning this particular command/trick/tip.
ANSWER:


----------------------
QUESTION: What are the dates and times of the CS107 midterm and final exams?
(Get those locked in on your calendar now!)
ANSWER: 


----------------------
QUESTION: Type any word into the search box on the Piazza forum.
At the top of the search results will appear a pop-up menu titled
"Search Tips". Check out the tips on this menu about forum searches.
How would you search for posts that contain either bit or byte?
ANSWER: 


----------------------
QUESTION: Please read the CS107 Honor Code and Collaboration policy 
and sign your name below to indicate you have done so.
(The web page is http://cs107.stanford.edu/collaboration.html)
ANSWER: I have read and understood the policy: [Your Name Here]


NOW RETURN TO THE ASSIGN0 WRITEUP
Read instructions on the intruder detection activity
and answer the questions below:

----------------------
QUESTION: What is the username of the intruder?
ANSWER: 

myth12:~/cs107/assign0/samples/server_image-91107/config> diff -a --suppress-common-lines -y trusted.bak.305 trusted.list
                                                              > mvaska

----------------------
QUESTION: What is the date/time when the trusted.list file was changed?
ANSWER: 

myth30:~/cs107/assign0/samples/server_image-91107/config> ls
total 10
drwxr-xr-x 2 zelenski operator 2048 Sep 21 14:06 .
drwxr-xr-x 5 zelenski operator 2048 Apr  6  2017 ..
-rw-r--r-- 1 zelenski operator 2822 Aug 22 17:21 trusted.bak.305
-rw-r--r-- 1 zelenski operator 2829 Sep 21 14:06 trusted.list

----------------------
QUESTION: List the names of the programs in the server_image_91107/bin/ 
directory that appear (based on timestamp) to have been edited by the
intruder. 
ANSWER: 

-rwx------ 1 zelenski operator  11598 Sep 21 14:11 fishy
-rwx------ 1 zelenski operator  11598 Sep 21 14:11 baash
-rwx------ 1 zelenski operator  11598 Sep 21 14:12 rkit
-rwx------ 1 zelenski operator  11598 Sep 21 14:12 shellh

----------------------
QUESTION: List the usernames of all the users whose init.d files
appear to have been compromised by the intruder.
ANSWER: 

myth12:~/cs107/assign0/samples/server_image-91107> grep -e fishy -e baash -e rkit -e shellh -r user
user/edgestrips/init.d:fishy
user/fennelturnips/init.d:baash
user/observantben/init.d:baash
user/poundreflect/init.d:shellh
user/unfastencutter/init.d:said ${ski -nk} | rkit
user/woolerduty/init.d:baash | rkit
user/yangoncurrency/init.d:rkit

----------------------

