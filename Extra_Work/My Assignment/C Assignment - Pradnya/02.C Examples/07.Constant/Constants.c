#include<stdio.h>

#define MY_PI 3.1415926535897932

#define AMC_STRING "ASTROMEDICOMP RTR 3.0"

enum
{
    SUNDAY,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY
};

enum
{
    JANUARY = 1,
    FEBRUARY,
    MARCH,
    APRIL,
    MAY,
    JUNE,
    JULY,
    AUGUST,
    SEPTEMBER,
    OCTOMBER,
    NOVEMBER,
    DECEMBER

};

enum Numbers
{
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE = 5,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN
};

enum boolean
{
    TRUE = 1,
    FALSE = 0
};

int main(void)
{
    const double epsilon = 0.000001;

    printf("\n\n");
    printf("Local Contant Epsilon = %lf\n\n", epsilon);

    printf("Sunday is day number = %d\n",SUNDAY);
    printf("Monday is day number = %d\n",MONDAY);
    printf("Tuesday is day number = %d\n",TUESDAY);
    printf("Wednesday is day number = %d\n",WEDNESDAY);
    printf("Thursday is day number = %d\n",THURSDAY);
    printf("Friday is day number = %d\n",FRIDAY);
    printf("Saturday is day number = %d\n",SATURDAY);
    printf("Thursday is day number = %d\n\n",THURSDAY);

    printf("One is Enum Number = %d\n", ONE);
    printf("Two is Enum Number = %d\n", TWO);
    printf("Three is Enum Number = %d\n", THREE);
    printf("Four is Enum Number = %d\n", FOUR);
    printf("Five is Enum Number = %d\n", FIVE);
    printf("Six is Enum Number = %d\n", SIX);
    printf("Seven is Enum Number = %d\n", SEVEN);
    printf("Eight is Enum Number = %d\n", EIGHT);
    printf("Nine is Enum Number = %d\n", NINE);
    printf("Ten is Enum Number = %d\n\n", TEN);

    printf("January's Month Number = %d\n",JANUARY);
    printf("Frebruary's Month Number = %d\n",FEBRUARY);
    printf("March's Month Number = %d\n",MARCH);
    printf("April's Month Number = %d\n",APRIL);
    printf("May's Month Number = %d\n",MAY);
    printf("June's Month Number = %d\n",JUNE);
    printf("Jully's Month Number = %d\n",JULY);
    printf("August's Month Number = %d\n",AUGUST);
    printf("September's Month Number = %d\n",SEPTEMBER);
    printf("Octomber's Month Number = %d\n",OCTOMBER);
    printf("November's Month Number = %d\n",NOVEMBER);
    printf("December's Month Number = %d\n\n",DECEMBER);

    printf("Value of TRUE is = %d\n", TRUE);
    printf("Value of FALSE is = %d\n", FALSE);

    printf("MY_PI Micro value = %.101f\n\n", MY_PI);
    printf("Area Of Circle of Radius 2 units = %f\n\n", (MY_PI * 2.0f * 2.0f));

    printf("\n\n");

    printf(AMC_STRING);
    printf("\n\n");

    printf("AMC_STRING is :%s\n",AMC_STRING);
    printf("\n\n");

    return(0);
    
}
