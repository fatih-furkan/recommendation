# recommendation

It is a c program that takes a text file that contains user ids, film ids, and film ratings. Program have different types of recommendation systems. If one wants to make a change at example.txt, he have to change MAX_LINE, USER_NUM and MOVIE_NUM values with new appropriate values. User ids start from 1 and they are numareted from 1 to USER_NUM, film ids are from 1 to MOVIE_NUM. All the numbers between 1 and USER_NUM must be used, same for the MOVIE_NUM with film ids. 

MAX_LINE is the number of text file's lines. 
REC_ARR_SIZE is number of element of the array used for making recommendations, can be changed according to needs.

Inappropriate Inputs
----
- Only integers can be entered when program asks ids. If the entered number is smaller then 1, it is accepted as 1, and if it is bigger than USER_NUM it is accepted as USER_NUM.
