How to use "determine"?

    First, make sure that prover9 and mace4 exist in "usr/local/bin".
        
        They can be compiled from https://github.com/ai4reason/Prover9 by cloning into the repository and following the steps inside the README.make file.
        Then from within the "Prover9/bin" folder execute the following commands:
            sudo cp prover9 /usr/local/bin/prover9
            sudo cp mace4 /usr/local/bin/mace4

    Second, use "make determine" to compile the program inside the root folder of this repository.

    Third, use "./determine [YOUR_FILE]" to determine whether your knot is a knot or an unknot.


What is "knot"?

    The knot program will help you more easily generate knot files for the determine program.


How to compile "knot"?

    Use the "make knot" command inside the root directory of this repository.


How to use "knot"?

    The "knot.c" source file already contains some predefined representations for the trefoil, cinquefoil and septafoil knots.
    These representations work in the following way:
        For the TREFOIL_KNOT:
            1 * 2 = 3 * 1 = 2 * 3 = 1
        
            This means that arc 1 goes under arc 2 and becomes arc 3.
            Then arc 3 goes under arc 1 and becomes arc 2.
            Finally arc 2 goes under arc 3 and becomes arc 1 again.

            _______
      1 -> /       \
          |         |
           \   ____________
            \ /     |      \
             \      |       | <- 3
            / \     |       |
           /   \___ | _____/
          |         |
      2 -> \_______/

    --------------------------------------------------------------------------------------------------------------------------

    7   #define TREFOIL_KNOT { 1, 2, 3, 1, 2, 3, 1 }
    8   #define CINQUEFOIL_KNOT { 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1 }
    9   #define SEPTAFOIL_KNOT { 1, 2, 3, 4, 5, 6, 7, 1, 2 ,3, 4, 5, 6, 7, 1 }

    --------------------------------------------------------------------------------------------------------------------------

    Executing the knot program will create a file named "file.in" that contains the representation of the TREFOIL_KNOT and the
    goal to demonstrate that all of these arcs are actually the same singular arc ( equivalent to being the unknot ).

    To generate a file for any knot, you will need a pen and paper to determine how to represent your knot in this format.
    You will need to draw your knot and label each of the arcs with a number of your choice ( an arc is any continuous line in your drawing ).
    Then starting from any arc, follow it and write the first arc that it goes under and the arc that it turn into:

        { 1, 2, 3 }

            2
            |
            |
      1 ----|---- 3
            |
            |

    Then continue on with the arc that your arc has turned into. ( In the above case, arc 3 )
    Your list of arcs should end with the same arc that it started with.

    So, to have a valid representation of a knot, the following must be true about your list:
        
        Your list must start and end with the same arc:  { 1, 2, 3, ..., 1 }

        An arc cannot appear twice at an even index in your list: { 1, 2, 1, ... } IMPOSSIBLE ARC!!!
            Exception: The arc that your list starts with will appear again at the end of your list!

        Every arc at an odd index in your list must also appear at an even index in your list: { 1, 2, 1 } LINK, NOT KNOT!!!
            A link is a type of knot made of more than one string.

        The length of you list must be equal to 2 * NO_OF_ARCS + 1

    The validate_knot function checks if your knot is valid using these criterion above.

Hope this helps, bye!
