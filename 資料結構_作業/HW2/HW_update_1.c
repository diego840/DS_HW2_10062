#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define max 1050
//  char
char shape[max]; //存每行input的share (字串型態)
char *token; //strtok中的token
//  int
int resource_y,resource_x,req;
//存一條ID中各種型態的shape !use function:[input,Row_sorting,swap,Column_sorting,Print]
int candidate_shape[max][max]; // [Y][X] 
int shape_num[max]; //存一條ID中有多少組的Shape !use function:[input,Row_sorting,swap,Print]
int original_ID[max]; //存sorting前的ID !use function:[input,swap,Print]
int tmp_list[max];  //tmp list !use function:[Row_sorting,swap]
int mode; //存sorting的排列模式 (X >= Y)=>1   (X < Y)=>0
int counter_0[5] = {1,2,4,8,16};
int counter_1[5] = {16,8,4,2,1};
//int counter_Y_0[5][max];
//int counrer_Y_1[5][max];
int insert_index[max];    
int insert_idx = 0; //insert_index的index
int result; //接收回傳的結果 1:代表shape能通過 0:代表shape不能通過
int accept_ID[max][5]; //存打印出來的 [0]:ID [1]:shape_y [2]:shape_x [3]:座標X軸 [4]:座標Y軸
int accept_index = 0; //accpet_ID的index

//  Node
struct node{
    int all_shape_Y_aft,all_shape_X_aft;
    int me_shape_Y,me_shape_X;
    int src_X,src_Y;
    struct node* left;
    struct node* right;
};
//  Create a new Node
struct node* createNode(int Y,int X,int src_X,int src_Y){
    struct node* newNode = malloc(sizeof(struct node));
    newNode->all_shape_Y_aft = Y;
    newNode->all_shape_X_aft = X;
    newNode->me_shape_Y = 0;
    newNode->me_shape_X = 0;
    newNode->src_X = src_X;
    newNode->src_Y = src_Y;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
};
//  root
struct node* root;
//  輸入
void input(){
    scanf("%d %d %d",&resource_y,&resource_x,&req);
    for (int i = 0; i < req; i++){
        scanf("%*c %[^\n]",shape);
        int req_index = 0;
        token = strtok(shape,"x ");
        while (token != NULL){
            if (req_index > 0){
                candidate_shape[i][req_index-1] = atoi(token);
            }
            req_index++;
            token = strtok(NULL,"x ");
        }
        shape_num[i] = req_index - 1;
        original_ID[i] = i;
        candidate_shape[i][req_index-1] = -1;
    }
    if (resource_x >= resource_y ){
        mode = 1;
    }else{
        mode = 0;
    }
}
//  同行share排序,row sorting
int cmpmin(const void * a, const void * b){
    return( *(int*)a - *(int*)b );    
}
int cmpmax(const void * a,const void * b){
    return( *(int*)b - *(int*)a );
}
void Row_Sorting(int left,int right,int order){
    for (int i = left; i <= right; i++){
        if (shape_num[i] > 2){
            for (int j = 0,jj = 0; candidate_shape[i][j] != -1; j = j+2,jj++){
                tmp_list[jj] = candidate_shape[i][j];
            }
            if (order != 0){
                qsort(tmp_list,(shape_num[i]/2),sizeof(int),cmpmax);
            }else{
                qsort(tmp_list,(shape_num[i]/2),sizeof(int),cmpmin);
            }
            for (int k = 0,kk = 0; candidate_shape[i][k] != -1;k = k+2,kk++){
                candidate_shape[i][k] = tmp_list[kk];
                candidate_shape[i][k+1] = resource_y/tmp_list[kk];
            }
        }      
    }
}
//  不同行shape排序,column sorting
void swap(int x, int y){
    int swap_tmp;
    memset(tmp_list,0,sizeof(tmp_list));
    memcpy(tmp_list,candidate_shape[x],sizeof(candidate_shape[x]));
    memcpy(candidate_shape[x],candidate_shape[y],sizeof(candidate_shape[x]));
    memcpy(candidate_shape[y],tmp_list,sizeof(candidate_shape[x]));
    swap_tmp = shape_num[x];
    shape_num[x] = shape_num[y];
    shape_num[y] = swap_tmp;
    swap_tmp = original_ID[x];
    original_ID[x] = original_ID[y];
    original_ID[y] = swap_tmp;
}
void Column_Sorting(int left,int right, int order){
    if (left < right){
        int formula = (left+right)/2;
        int pivot = candidate_shape[formula][0];
        int i = left-1;
        int j = right+1;

        while (1){
            if (order != 0){
                while(candidate_shape[++i][0] > pivot);
                while(candidate_shape[--j][0] < pivot);
            }else{
                while(candidate_shape[++i][0] < pivot);
                while(candidate_shape[--j][0] > pivot);
            }
            
            if (i >= j){
                break;
            }
            swap(i,j);
        }
        Column_Sorting(left,i-1,order);
        Column_Sorting(j+1,right,order);
    }
}
//  打印
void Print(){
    /*
    for (int i = 0; i < req; i++){
        for (int j = 0; candidate_shape[i][j] != -1; j++){
            printf("%d ",candidate_shape[i][j]);   
        }
        printf("\nshape_num: %d original_ID:%d",shape_num[i],original_ID[i]);
        printf("\n");
    }
    printf("\n");*/
    printf("%d\n",accept_index);
    for (int i = 0; i < accept_index; i++){
        printf("%d ",original_ID[accept_ID[i][0]]);
        printf("%dx%d ",accept_ID[i][1],accept_ID[i][2]);
        printf("%d %d\n",accept_ID[i][3],accept_ID[i][4]);
    }
}
//  將符合的shape加入tree同時創建新的空間
struct node* Insert_tree(struct node* root,int Y,int X){
    int left_Y = root->all_shape_Y_aft - Y;
    int left_X = X;
    int right_Y = root->all_shape_Y_aft;
    int right_X = root->all_shape_X_aft - X;
    root->me_shape_Y = Y;
    root->me_shape_X = X;
    root->left = createNode(left_Y,left_X,(root->src_X),(root->src_Y+Y));
    root->right = createNode(right_Y,right_X,(root->src_X+X),(root->src_Y));

    return root;
}
//  Node ()
int check_shape(struct node* root,int Y,int X,int user_id,int order){
    if (Y <= root->all_shape_Y_aft && X <= root->all_shape_X_aft){
        if (root->left == NULL && root->right == NULL){
            //printf("all_shape_Y:%d all_shape_X:%d\n",root->all_shape_Y_aft,root->all_shape_X_aft);
            struct node* accept_node = Insert_tree(root,Y,X);
            accept_ID[accept_index][0] = user_id;
            accept_ID[accept_index][1] = accept_node->me_shape_Y;
            accept_ID[accept_index][2] = accept_node->me_shape_X;
            accept_ID[accept_index][3] = accept_node->src_Y;
            accept_ID[accept_index][4] = accept_node->src_X;
            accept_index++;
            return 1;
        }else{
            if (order == 1 && check_shape(root->right,Y,X,user_id,order) == 1){
                return 1;
            }else if (order == 0 && check_shape(root->right,Y,X,user_id,order) == 1){
                return 1;
            }else{
                if (order == 1 && check_shape(root->left,Y,X,user_id,order) == 1){
                    return 1;
                }else if (order == 0 && check_shape(root->left,Y,X,user_id,order) == 1){
                    return 1;
                }                
            }
        }
    }
    return 0;
}
//
void visit_anyshape(struct node* root,int order){
    for (int i = 0; i < 5; i++){
        for (int req_i = 0; req_i < req; req_i++){
            if (candidate_shape[req_i][insert_index[req_i]] == counter_1[i] && insert_index[req_i] != -1 && order == 1){
                int tmp_1 = candidate_shape[req_i][insert_index[req_i]];
                result = check_shape(root,tmp_1,(16/tmp_1),req_i,order);
                if (result == 1){
                    insert_index[req_i] = -1;    
                }else{
                    insert_index[req_i] += 2;
                }
            }else if (candidate_shape[req_i][insert_index[req_i]] == counter_0[i] && insert_index[req_i] != -1 && order == 0){
                int tmp_0 = candidate_shape[req_i][insert_index[req_i]];
                result = check_shape(root,tmp_0,(16/tmp_0),req_i,order);
                if (result == 1){
                    insert_index[req_i] = -1;
                }else{
                    insert_index[req_i] += 2;
                }
            }
        }
    }
}
//  main
int main(){
    memset(insert_index,0,sizeof(insert_index));
    input();
    root = createNode(resource_y,resource_x,0,0);
    Row_Sorting(0,req-1,0);
    Column_Sorting(0,req-1,0);
    visit_anyshape(root,0);
    Print();
    return 0;
}