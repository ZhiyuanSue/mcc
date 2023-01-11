int a[2]={1,2};
int b={1};
int c[1]={1,2};
struct tmpa{
    int a;
    int b;
};
struct tmpa tmp=(struct tmpa){1,2};
struct{
        int a[3],b;
    } w[]={ {1},2,3,4,{5,6} ,[3]=7,8};
struct{
    int a;
    double b;
} x={.b=2.0};
int y[4][3] = {
    1, 3, 5, 2, 4, 6, 3, 5, 7
};