# Log Diary

## :zap: Understand the basic structure and approach:zap:
**25/02/2022 Dummies..** :confused:
1. Research online about the lexer and parser for the C90
2. Understand how yylex, yyin, yyparse works with extern and transmit information.
3. Look through Lab 1 and Lab 2 to get some idea of the structure.

**26/02/2022 Research** :blue_book:
1. Found this website for the lexer:
    https://www.lysator.liu.se/c/ANSI-C-grammar-l.html
2. Found this website for the parser:
    https://www.lysator.liu.se/c/ANSI-C-grammar-y.html
3. Lexer logic is pretty clear. :)
4. Try to understand the parser structure. But get confused about it. :(

**27/02/2022 Parser & AST Tree** :evergreen_tree:
1. Got to understand how functions/ expressions... flows through the parser and build up the ast tree.
        <p align="center">
        <img src="https://user-images.githubusercontent.com/62194910/156859167-79a39b7e-c21a-4ed8-806a-3c389bd9c546.png" width="500">
        </p>

**28/02/2022 Parser & AST Tree** :evergreen_tree:
1. Starts to write the top few layers of the parser and the ast tree. 
2. Tried to use GodBolt to generate some MIPS code to see the pattern. Seems like there is something called stack used for functions.

**01/03/2022 Stack** :pencil2:
1. Useful website for the stack:
    http://math-atlas.sourceforge.net/devel/assembly/mipsabi32.pdf#page=27
2. Got some basic understanding of stack by analysing the MIPS code step by step.
     <p align="center">
     <img src="https://user-images.githubusercontent.com/62194910/156859953-2d474ed1-1579-4675-ac64-45ae408a135b.jpg" width="850">
     </p>

**02/03/2022 Stack** :fist:
1. Realized that there is a need to build up a context class for each stack. Before the Generate MIPS operation, we need to traverse throught the stack to get following information (as far as we know, might be insufficient). These information are needed to estimate the stack size.
    - How many initialize declarations.
    - Is there any function calls? 
    - How many parameters are passed into the function calls. (Consider later...) 
        Useful document: http://math-atlas.sourceforge.net/devel/assembly/mipsabi32.pdf#page=29
2. Decided to create a context class object for each scope (i.e. each function definition).

**03/03/2022 Context** :clipboard:
1. Built the structure of the context, which includes: stack size & variable bindings
2. Figured out how to build the context used for generate stacks and how to pass the class object context layer by layer to modify its content.
3. Should start to plan MIPS code generation. 

**04/03/2022 One testcase works** :tada:
1. Generate MIPS code for simple function like:
```
    int f(){
        int x = 0;
        return x;
    }
```
2. Generate MIPS code for function call like:
```
    int g(){
        int x = 1;
        return x;
    }
    int f() {
        int x;
        x = g();
        return x;
    }
```
3. Understand basically how Makefile and the testing bash for the compiler works.
4. Successfully test the testcase above with driver.c. 

## :zap: Get the basic features working:zap:
**05/03/2022 Expressions** :imp:
1. Write out the parser and MIPS code generation for all multiplicative expressions, additive expressions, shift expressions, relational expressions, equality expressions, and expressions, exclusive_or expressions, inclusive_or expressions, logical_and expressions. Including the case when they are nested together.
2. Only considered the case that $2 and $3 are sufficient for calculation, and there isn't a need for any temporary registers for now. (Need to append **Register Allocation** in the future).
3. Useful document for Register Allocation: http://math-atlas.sourceforge.net/devel/assembly/mipsabi32.pdf#page=24
4. Code like this can work:
```
    int f(){
        int x = 1;
        int y = 2;
        int m = 3;
        int z = x+y*z;
        return x||z;
    }
```
**06/03/2022 IfElse & While** :dizzy_face::repeat:
1. Finished unary assignment expression. Considered precedence. Seems work.
2. Finished If, IfElse, While, DoWhile. 
3. Although our code currently can fragily run the local variable inside while/ifelse block, it is not following the correct logic. This problem needs to be solved.  Context needs to be changed for local variables inside a scope. Don't have good idea yet.
    <p align="center">
    <img src="https://user-images.githubusercontent.com/83700530/156930939-86fb255f-7512-4ddf-8101-687998ba929a.png" width="150">
    </p>

**07/03/2022 Modify Context** :sleeping:
1. Rebuild context to address the situation where compound statement within compound statement. Basically the child context will inherit all variable bindings from its parent, and then change the current stack size after the child scope ends. In this way, the parent context will not have the variable binding information of the child context; while the child context will have all variable binding information of the parent context. Also, the child context will have no information of its brother/sister's context. Moreover, the stack size will change dynamically with new compound statement scope opening up. This stack size information will be passed all the way back to the function definition for generating the "opening & end lines" of a stack.
2. Too sleepy, will debug the modified context.cpp tomorrow...
    <p align="center">
    <img src="https://user-images.githubusercontent.com/83700530/157141703-52f8e954-85fc-448d-8c38-2711b1a9ad18.png" width="400">
    </p>
**09/03/2022 Context Rebuilt** :fire:
1. Debug the context class. Local variables inside compound statement can work without mistake.
2. Did function calls with more than 4 arguments. Figured out the way of using $4,$5,$6,$7 and stack memory on the caller function to pass in the arguments. This kind of function can work:
    <p align="center">
    <image src="https://user-images.githubusercontent.com/83700530/157582494-492f0131-9b46-48a3-bd71-98f8e56b6053.png" width="250">
    </p>

**10/03/2022 Precedence & ForLoop** :repeat:
1. Developed a new way to tell the precedence of expression. We assigned a precedence weight for each layer of expression. The value increases down the AST tree. (i.e. AddOperator = 10, MulOperator=11,...). When it reaches primary expression (i.e. Identifier, Int, Float,...) the value is set to zero. Then for all binary operator, we compare the left operand precedence weight and right operand precedence weight. If the right operand precedence weight is larger than the left operand operand weight. It means that we need to calculate the right operand expression first, so we flip the sequencye and generate MIPS for right operand expression first. For all other situations, we generate MIPS for left operand expression first by default.
2. Finish ~,!,++,-- (both post and pre) unary operators.
3. Finish For loop.
    <p align="center">
    <image src="https://user-images.githubusercontent.com/83700530/157791891-f92254c2-a3d2-4e9e-bde0-5a0ea2740a33.png" width="150">
    </p>

**11/03/2022 Register Allocation** :fire:
1. Found that what we did yesterday are useless and rubbish. We don't really need the priority method for the expression. It is not essentially "wrong". However, since we need utilize intermediate registers when generating MIPS for expressions with more operands (e.g. x*y+z*m<<e*t), we can just use a vector which contains 1 (currently using, cannot be touched) and 0 (currently not using, can use) symbol for each register, indicating whether they have been allocated or not. In this way we won't need to evaluate the priority and flip the sequence and so on...Our new logic is shown below:
        <p align="center">
        <image src = "https://user-images.githubusercontent.com/83700530/157994901-febc2014-f128-44f7-897c-8f3831543524.png" width = "350">
        </p>
2. Finish Switch and Global Variable.
The MIPS instructions for global variable produced in Godbolt didn't run successfully. We found out that we need to add .data & .text in MIPS for it to run.
.data section: declares variable names used in program; storage allocated in main memory (RAM)
.text section: contains program code (instructions)

**12/03/2022 Array** :fire:
1. Finished array declared globally and locally. 

**13/03/2022 Recursive Function & Continue** :arrows_clockwise:
1. Recursion requires the use of registers $16-23 to keep the values. To make it simpler to implement, we decided to move the register values on the stack at the beginning of each function call, and load them back to registers at the end of function calls, regardless of whether $16-23 are actually used. At the end of each callee fucntion, we need to move the value from $2 into the caller function's destination register. Haven't implement mutually recursive function call yet.
2. Finished continue by using a continue label: always place a label at the end of each 'while', 'do while', 'for' scope, so that when we see a 'continue' in the program, it can jump to that label

**14/03/2022 Enum** :smile_cat:
1. Looked into how enum works and decided that enum should be implemented mainly in the context code file. The context needs to 'remember' the enums defined in the global scope and retrieve the correct value in the later code.
            
**16/03/2022 Break & Struct** :office:
1. Break is implemented using labels - to jump to the end of the loop. To make the implementation easier, labels are created when a for/while loop is encountered, irrespective of whether it contains a break statement or not. Now, Switch can be fully tested with break. Minor problems about switch are then fixed.
2. We then started to look into struct and built the context class for struct to store the name of the struct and the names of its attributes. Not fully completed, to be continued tomorrow.
            
**17/03/2022 Struct continue** :arrow_right:
1. Finished struct today! Fully impelemented struct and also make sure that the compiler is compatible with struct defined in different format, ie. struct{int a;int b} c = {1,2} this type of struct declaration doesn't include a struct name, so we don't need to link the attributes to the struct name.
2. Fixed some problems with the build_context function
            
**18/03/2022 Pointer Basics** :point_left:
1. Implemented the basic functionality of pointer: deferencing, taking the address of a variable. 
            
**19/03/2022 Pointer Arithmetic** :heavy_plus_sign: :heavy_minus_sign:
1. Pointer arithmetic is a bit confusing. By doing some research and seeking help using github issue, we found that there are special behaviours if you are subtracting two pointers that are accessing the same structure (getting the offset). Otherwise you often have it that one of the pointers (the one on the right) is converted to an integer and used as an offset. This section explains the behaviour: http://port70.net/~nsz/c/c89/c89-draft.html#3.3.6
2. Most of the functionality of pointer have been implemented, however, we haven't worked on 'pointer to pointer' ie. **p. Will do it later.

**21/03/2022 Float** :alien:
1. We did some research on float data type. Some effort are place to convert it into float value labels. 
2. Float type variables use a different set of registers. For single-point float number, only the even number float registers are used. Special care has been taken when placing the return value of a float data type (in $f0 instead of $2).
3. Float instructions are slightly different from the 'normal' MIPS code. We tried it out in godbolt to understand the instructions.
            
**22/03/2022 Float** :boom:
1. Continue on finishing of the float data type.
            
**23/03/2022 ~ 26/03/2022 Short Pause** :zzz:
- Note: This is the last week of term and there are 2 labs and 1 project due in the week. Since we already have half of the advanced functionalities working, we decided to focus more time on the other labs and projects.
            
**27/03/2022 Double & Unsigned & Char** :two_women_holding_hands:
1. Started to work on double - perhaps the most difficult data type since it takes double the amount of space so stack size need to adapt to this accordingly. To recognise the double variable, we added extra information to the variable bindings in the context. 
2. unsigned data type is pretty easy to do. We finished it relatively quick.
3. Implemented character literals & string. This part is not as hard as we expected and was done relatively quick.

**28/03/2022 Double & sizeof & fix bits and bobs** :construction_worker:
1. Global double variables are declared in a special way. After some research and trying out, we successfully implemented this. There are still many functionalities that cannot adapt to double type:
    - double pointer and double array locally and globally, double as parameters, mixture of different types of parameters, etc. We will try to do as many as we can before the deadline.
2. Finished the 'pointer to pointer' scenario which was left out earlier.
3. Implemented the sizeof operator       
           
**29/03/2022 Sizeof & Type Define & Add More Float and Double functionalities & Extra testing** :tired_face:
1. Type define: Need to do the "lexer hack". Our parser need to maintain a list of identifiers that have been typedef'd. The lexer also has access to that list, and each time it lexes something that looks like an identifier, it sees if it appears in that list. If it does, then it lexes it as a typedef identifier, and if it doesn't, it lexes it as a variable identifier.
2. Implemented double pointer
3. Finished float array and tried to do double array 
4. Mixture of types used as function parameters: special care is needed when handling float and double parameters. Mixture of parameter types need to follow this rule  
![image](https://user-images.githubusercontent.com/62194910/169864123-555e73e5-349b-4f9d-b6d2-1e6e8a206be8.png)

        
            

