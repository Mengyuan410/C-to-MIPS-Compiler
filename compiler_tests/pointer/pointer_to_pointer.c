int f(){
   int  var;
   int  *ptr = &var;
   int  **pptr = &ptr;
   var = 30;  
   return **pptr;
}