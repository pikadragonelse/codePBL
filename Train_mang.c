#include <stdio.h>  
#include <math.h>
#include <stdlib.h>
#include <string.h>
//#include <time.h>
#define delta 0.0000001


float init_weight (); //Vì trọng số ban đầu được cho ngẫu nhiên nên dùng 1 hàm để tạo ngẫu nhiên trọng số

void weight(float w[][100], int num1, int num2); // Sử dụng hàm init_weight ở trên để khởi tạo giá trị ngẫu nhiên cho lần lượt các giá trị trọng số

float sigmoid(float x); // Hàm sigmoid dùng để tính toán nhiều lần trong chương trình

float dsigmoid( float x); //Đạo hàm sigmoid dùng để tính toán trong gradient descent

void bias( float b[], int num); // Vì hệ số bias là không đổi nên cố định luôn hệ số cho mạng (kết quả của tính toán thực nghiệm)

void Nhap(float a[], int num); 

void logistic_regression_1(float z[], float w[][100], float b[], float x[], int num1, int num2); // Dùng cho hiddenlayer

void logistic_regression_2(float z[], float w[][100], float b[], float x[], int num1, int num2); // Cần tạo 1 hàm tính công thức logistic regression (dùng cho ouputlayer)

void y_tinh_toan(float y[], float z[], int num); //Dùng 1 hàm để tính các giá trị sigmoid

void sai_so(float s[], float y1[], float y2[], int num); //Tính toán sai số so với thực tế (trăm tỷ VNĐ)

void tin_hieu_loi(float d[], float w[][100], float s[], int num1, int num2); // Nói cách khác là learning_rate

void  gradient_descent(float w[][100], float n, float d[], float Input[], float z[], int num1, int num2);

void IN(float y[], int num);

void read_file(FILE *file, float input[], int num);

void Ghi_data_Hidden_weight( float w[][100], int num1, int num2); //Dùng để lưu dữ liệu trọng số hidden vào file để dự báo

void Ghi_data_Output_weight( float w[][100], int num1, int num2); //Dùng để lưu dữ liệu trọng số output vào file để dự báo


/* Nên xác định các kích thước của mạng để dễ sử dụng (Dùng static vì tránh sự thay đổi không muốn của biến trong cả quá trình chạy chương trình đồng thời để tiện thay đổi kích thước của mạng sau này) */
static const int numInputs = 2;
static const int numHiddenNodes = 2;
static const int numOutput = 1;
static const float n = 1; // Trong Gradient descent có  dùng đến hằng số học nên dựa vào thực nghiệm chúng ta chọn giá trị cho hằng số học


int main(){
    //Cần phải sử dụng các mảng tính toán nên phải phân bổ các mảng cho các lớp, độ lệch và trọng số

    float hiddenWeight [numHiddenNodes] [numInputs];   /* đặt số lượng hàng cột của mảng như thế này để tiện tính toán */
    float ouputWeight [numOutput] [numHiddenNodes];    /* đặt số lượng hàng cột của mảng như thế này để tiện tính toán */
    
    float hiddenBias [numHiddenNodes];
    float outputBias [numOutput];
    
    bias(hiddenBias, numHiddenNodes);
    bias(outputBias, numOutput);
    
    //Lấy dữ liệu dùng để train mạng (trăm tỷ VNĐ)
    int p;
    printf("Chon phuong thuc nhap data de train mang (1: nhap tu ban phim, 2: tu file): ");
    scanf("%d", &p);
    
    float Input[numInputs];
    float y_thuc_te[numOutput];
    
    if (p == 1 ){
        printf("Nhap Input(%d ngay lien tiep): ", numInputs);
        Nhap(Input, numInputs); 
        printf("\nNhap y thuc te: ");
        Nhap(y_thuc_te, numOutput);
    }
    if ( p == 2) {
        char address1[1000];
        char address2[1000];
        FILE *input;
        FILE *output;
        int dem = 0;
        int check;
        
        {   
            if (dem > 3){
                printf("ERORR !!, thu chay lai chuong trinh");
                exit(1);
            }    

            if (dem == 0){
                printf("Nhap dia chi file input de train mang (dung \\\\ thay vi  \\ de phan cach giua cac phan trong dia chi): \n ");
                fflush(stdin);
                gets(address1);
                
                printf("\n Nhap dia chi file output mong muon de train mang (dung \\\\ thay vi  \\ de phan cach giua cac phan trong dia chi): \n");
                fflush(stdin);
                gets(address2);
            }
            else {
                
                printf("Nhap dia chi sai hoac file trong, vui long nhap lai dia chi");

                printf("Nhap dia chi file input de train mang (dung \\\\ thay vi  \\ de phan cach giua cac phan trong dia chi): ");
                fflush(stdin);
                gets(address1);
                
                printf("\nNhap dia chi file output mong muon de train mang (dung \\\\ thay vi  \\ de phan cach giua cac phan trong dia chi)");
                fflush(stdin);
                gets(address2);
            }
            input = fopen(address1, "r");
            output = fopen(address2, "r");
            dem++;
        }
        while ( input == NULL || output == NULL);

        read_file(input, Input, numInputs);
        read_file(output, y_thuc_te, numOutput);
        
        fclose(input);
        fclose(output);

    }

    if (p != 1 && p != 2){
        printf("ERORR !");
        exit(0);
    }
    
    //srand((int)time(0));
    weight(hiddenWeight, numHiddenNodes, numInputs);
    weight(ouputWeight, numOutput, numHiddenNodes);
    
    
    // Tính toán với hàm sigmoid cho các noron ở hiddenlayer và output layer
    float z_Hiddens[numHiddenNodes];
    float z_Output[numOutput]; 
    
    float y_Hiddens[numHiddenNodes];
    float y_Output[numOutput];
    
    float s[numOutput];
    float d[numHiddenNodes];
    
    while (1){
        logistic_regression_1(z_Hiddens, hiddenWeight, hiddenBias, Input, numHiddenNodes, numInputs);
        y_tinh_toan(y_Hiddens, z_Hiddens, numHiddenNodes);
    
        logistic_regression_2(z_Output, ouputWeight, outputBias, y_Hiddens, numOutput, numHiddenNodes);
        y_tinh_toan(y_Output, z_Output, numOutput);

        sai_so(s, y_thuc_te, y_Output, numOutput);
        
        if (kiemtra(s, numOutput) == 0 ) break;

        tin_hieu_loi(d, ouputWeight, s, numOutput, numHiddenNodes);

        gradient_descent(hiddenWeight, n, d, Input, z_Hiddens, numHiddenNodes, numInputs);
        gradient_descent(ouputWeight, n, s, y_Hiddens, z_Output, numHiddenNodes, numOutput);
    }
    
    IN(y_Output, numOutput);
    
    Ghi_data_Hidden_weight(hiddenWeight, numHiddenNodes, numInputs);
    Ghi_data_Output_weight(ouputWeight, numOutput, numHiddenNodes);
    
    return 0;
}

float init_weight () {
    return  (float)rand() / (float) RAND_MAX;
}

void weight(float w[][100], int num1, int num2){
    int i,j;
    for (i = 0; i < num1; i++){ 
        for (j = 0; j < num2; j++){
            w[i][j] = init_weight();
        }
    }
}

float sigmoid(float x){
    return 1 / (1 + exp(-x));
}


float dsigmoid( float x){
    return exp(-x) / pow((1 + exp(-x)), 2);
}

void bias( float b[], int num){
    int i;
    for (i = 0; i < num; i++){
        b[i] = 0.1;
    }
}

void Nhap(float a[], int num){
    int i;
    for (i = 0; i < num; i++){
        scanf("%f", &a[i]);
    }
}

void logistic_regression_1(float z[], float w[][100], float b[], float x[], int num1, int num2){
    int i, j;
    float sum[101];
    for (i = 0; i < num1; i++){
        for (j = 0; j < num2; j++){
            sum[j] += x[i]*w[i][j];
        }
    }
    for (i = 0; i < num1; i++){
        z[i] = sum[i] + b[i];
    }
}

void logistic_regression_2(float z[], float w[][100], float b[], float x[], int num1, int num2){
    int i, j;
    for (i = 0; i < num1; i++){
        float sum = 0;
        for (j = 0; j < num2; j++){
            sum += x[j]*w[i][j]; 
        }
        z[i] = sum + b[i];
    }
}

void y_tinh_toan(float y[], float z[], int num){
    int i;
    for (i = 0; i < num; i++){
        y[i] = sigmoid(z[i]);
    }
}

void sai_so(float s[], float y1[], float y2[], int num){
    int i;
    for (i = 0; i < num; i++){
        s[i] = y1[i] - y2[i];
    }
}
void tin_hieu_loi(float d[], float w[][100], float s[], int num1, int num2){
    int i, j;
    for (i = 0; i < num1; i++){
        for (j = 0; j < num2; j++){
            d[j] = w[i][j] * s[i];
        }
    }
}
void  gradient_descent(float w[][100], float n, float d[], float Input[], float z[], int num1, int num2){
    int i, j;
    float t[101];
    for (i = 0; i < num2; i++){
        t[i] = n*d[i]*dsigmoid(z[i])*Input[i];
    }
    for (i = 0; i < num1; i++){
        for (j = 0; j <num2; j++){
            w[i][j] += t[j];
        }
    }
}
int kiemtra(float s[], int num){
    int i;
    int dem = 0;
    for (i = 0; i < num; i++){
        if (fabs(s[i]) > delta) dem++ ;
    }
    return dem;
}
void IN(float y[], int num){
    int i;
    for (i = 0; i < num; i++){
        printf("%.7f", y[i]);
    }
}

void Ghi_data_Hidden_weight( float w[][100], int num1, int num2){
    int i, j;
    FILE *file = fopen ("C:\\Users\\HP\\Desktop\\Hiddenweight.txt", "w");
    for (i = 0; i < num1; i++){
        for (j = 0; j < num2; j++){
            fprintf(file, "%f ", w[i][j]);
        }
    }
    fclose(file);
}

void Ghi_data_Output_weight( float w[][100], int num1, int num2){
    int i, j;
    FILE *file = fopen ("C:\\Users\\HP\\Desktop\\Outputweight.txt", "w");
    for (i = 0; i < num1; i++){
        for (j = 0; j < num2; j++){
            fprintf(file, "%f ", w[i][j]);
        }
    }
    fclose(file);
}

void read_file(FILE *file, float input[], int num){
    int i;
    for (i = 0; i < num; i++){
        fscanf(file, "%f", &input[i]);
    }
}

