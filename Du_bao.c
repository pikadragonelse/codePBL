#include <stdio.h>  
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

float sigmoid(float x); // Hàm sigmoid dùng để tính toán nhiều lần trong chương trình

void bias( float b[], int num); // Vì hệ số bias là không đổi nên cố định luôn hệ số cho mạng (kết quả của tính toán thực nghiệm)

void Nhap(float a[], int num);

void logistic_regression_1(float z[], float w[][100], float b[], float x[], int num1, int num2); // Dùng cho hiddenlayer

void logistic_regression_2(float z[], float w[][100], float b[], float x[], int num1, int num2); // Cần tạo 1 hàm tính công thức logistic regression (dùng cho ouputlayer)

void y_tinh_toan(float y[], float z[], int num); //Dùng 1 hàm để tính các giá trị sigmoid

void IN(float y[], int num);

void Ghi_data_Ngay( float y[], int num1); // Ghi dữ liệu của ngày dự báo được vào file.

void read_file_input(FILE *file, float input[], int num);

void chuyen_input(float input1[], float input2[], int num); // chuyển input từ mảng trung gian qua mang Input chính để sử dụng

void read_file_weight(FILE *file, float w[][100], int num1, int num2);

/* Nên xác định các kích thước của mạng để dễ sử dụng (Dùng static vì tránh sự thay đổi không muốn của biến trong cả quá trình chạy chương trình đồng thời để tiện thay đổi kích thước của mạng sau này) */
static const int numInputs = 4;
static const int numHiddenNodes = 4;
static const int numOutput = 1;

int main(){
    //Cần phải sử dụng các mảng tính toán nên phải phân bổ các mảng cho các lớp, độ lệch và trọng số

    float hiddenWeight [numHiddenNodes] [numInputs];   /* đặt số lượng hàng cột của mảng như thế này để tiện tính toán */
    float outputWeight [numOutput] [numHiddenNodes];    /* đặt số lượng hàng cột của mảng như thế này để tiện tính toán */
    
    float hiddenBias [numHiddenNodes];
    float outputBias [numOutput];
    
    bias(hiddenBias, numHiddenNodes);
    bias(outputBias, numOutput);
    
    //Lấy dữ liệu dùng để dự đoán (trăm tỷ VNĐ)
    int p;
    printf("Chon phuong thuc nhap du lieu de du doan (1: nhap tu ban phim, 2: tu file): ");
    scanf("%d", &p);

    float Input[numInputs];
    

    if (p == 1){
        int choice;
        printf("Ban muon du bao 1 ngay hay nhieu ngay lien tiep? (1: mot ngay, 2: nhieu ngay lien tiep): ");
        scanf("%d", &choice);
        if (choice == 1){
            printf("Nhap doanh thu cua %d ngay truoc ngay muon du bao: ", numInputs);
            Nhap(Input, numInputs);
        }
        if (choice == 2){
            printf("Nhap doanh thu cua %d ngay truoc ngay muon du bao: ", numInputs);
            Nhap(Input, numInputs);
            Ghi_data_Ngay(Input, numInputs);
            int ngay;
            float Input_file[30];
            FILE *doanhthu;
            doanhthu = fopen("C:\\Users\\HP\\Desktop\\doanhthu.txt", "r");
            printf("Ban muon du bao toi ngay thu: ");
            scanf("%d", &ngay);
            int i = 2;
            while (i < ngay){
                i++;
                read_file_input(doanhthu, Input_file, i);

                chuyen_input(Input, Input_file, i - 1);

                FILE *weight_hidden;
                FILE *weight_output;
                weight_hidden = fopen("C:\\Users\\HP\\Desktop\\Hiddenweight.txt", "r");
                weight_output = fopen("C:\\Users\\HP\\Desktop\\Outputweight.txt", "r");

                read_file_weight (weight_hidden, hiddenWeight, numHiddenNodes, numInputs);
                read_file_weight (weight_output, outputWeight, numOutput, numHiddenNodes);
    
                fclose (weight_hidden);
                fclose (weight_output);
    
                float z_Hiddens[numHiddenNodes];
                float z_Output[numOutput]; 
    
                float y_Hiddens[numHiddenNodes];
                float y_Output[numOutput];
    
                logistic_regression_1(z_Hiddens, hiddenWeight, hiddenBias, Input, numHiddenNodes, numInputs);
                y_tinh_toan(y_Hiddens, z_Hiddens, numHiddenNodes);
    
                logistic_regression_2(z_Output, outputWeight, outputBias, y_Hiddens, numOutput, numHiddenNodes);
                y_tinh_toan(y_Output, z_Output, numOutput);
    
                Ghi_data_Ngay(y_Output, numOutput); 

                IN(y_Output, numOutput);
            }
            fclose(doanhthu);
            exit(0);
        }
    }
    if (p == 2) {
        char address[1000];
        FILE *doanhthu;
        int dem = 0;
        do{
            if (dem == 0){    
                printf("Nhap dia chi cua file du lieu doanh thu (dung \\\\ thay vi  \\ de phan cach giua cac phan trong dia chi): \n");
                fflush(stdin);
                gets(address);
            }
            else {
                printf("Nhap dia chi sai hoac file trong, nhap lai\n");
                printf("Nhap dia chi cua file du lieu doanh thu (dung \\\\ thay vi  \\ de phan cach giua cac phan trong dia chi): \n");
                fflush(stdin);
                gets(address);
            }
            doanhthu = fopen(address,"r");
            dem++;
        }
        while (doanhthu == NULL);

        int ngay;
        int choice;
        float Input_file[30];
        printf("Ban muon du bao 1 ngay hay nhieu ngay lien tiep? (1: mot ngay, 2: nhieu ngay lien tiep): ");
        scanf("%d", &choice);
        
        if (choice == 1){
            printf("Nhap ngay muon du bao: ");
            scanf("%d", &ngay);
        
            read_file_input(doanhthu, Input_file, ngay);
        
            chuyen_input(Input, Input_file, ngay - 1); // vì mảng chạy từ số 0 nên phải trừ ngày đi 1
        }
        if (choice == 2){
            printf("Ban muon du bao toi ngay thu: ");
            scanf("%d", &ngay);
            int i = 2;
            while (i < ngay){
                i++;
                read_file_input(doanhthu, Input_file, i);

                chuyen_input(Input, Input_file, i - 1);

                FILE *weight_hidden;
                FILE *weight_output;
                weight_hidden = fopen("C:\\Users\\HP\\Desktop\\Hiddenweight.txt", "r");
                weight_output = fopen("C:\\Users\\HP\\Desktop\\Outputweight.txt", "r");

                read_file_weight (weight_hidden, hiddenWeight, numHiddenNodes, numInputs);
                read_file_weight (weight_output, outputWeight, numOutput, numHiddenNodes);
    
                fclose (weight_hidden);
                fclose (weight_output);
    
                float z_Hiddens[numHiddenNodes];
                float z_Output[numOutput]; 
    
                float y_Hiddens[numHiddenNodes];
                float y_Output[numOutput];
    
                logistic_regression_1(z_Hiddens, hiddenWeight, hiddenBias, Input, numHiddenNodes, numInputs);
                y_tinh_toan(y_Hiddens, z_Hiddens, numHiddenNodes);
    
                logistic_regression_2(z_Output, outputWeight, outputBias, y_Hiddens, numOutput, numHiddenNodes);
                y_tinh_toan(y_Output, z_Output, numOutput);
    
                Ghi_data_Ngay(y_Output, numOutput); 

                IN(y_Output, numOutput);
            }
            fclose(doanhthu);
            exit(0);
        }
        if (choice != 1 && choice != 2 ) {
            printf("ERORR !");
            exit(0);
        }
    }

    if (p != 1 && p != 2){
        printf("ERORR !");
        exit(0);
    }

    //Lấy dữ liệu trọng số weight từ file sau khi đã train mạng
    FILE *weight_hidden;
    FILE *weight_output;
    weight_hidden = fopen("C:\\Users\\HP\\Desktop\\Hiddenweight.txt", "r");
    weight_output = fopen("C:\\Users\\HP\\Desktop\\Outputweight.txt", "r");

    read_file_weight (weight_hidden, hiddenWeight, numHiddenNodes, numInputs);
    fclose (weight_hidden);
    
    read_file_weight (weight_output, outputWeight, numOutput, numHiddenNodes);
    fclose (weight_output);
    
    float z_Hiddens[numHiddenNodes];
    float z_Output[numOutput]; 
    
    float y_Hiddens[numHiddenNodes];
    float y_Output[numOutput];
    
    logistic_regression_1(z_Hiddens, hiddenWeight, hiddenBias, Input, numHiddenNodes, numInputs);
    y_tinh_toan(y_Hiddens, z_Hiddens, numHiddenNodes);
    
    logistic_regression_2(z_Output, outputWeight, outputBias, y_Hiddens, numOutput, numHiddenNodes);
    y_tinh_toan(y_Output, z_Output, numOutput);
    
    Ghi_data_Ngay(y_Output, numOutput); 

    IN(y_Output, numOutput);

     
    return 0;
}



float sigmoid(float x){
    return 1 / (1 + exp(-x));
}

void bias( float b[], int num){
    int i;
    for (i = 0; i < num; i++){
        b[i] = 1;
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

void IN(float y[], int num){
    int i;
    for (i = 0; i < num; i++){
        printf("%.7f \n", y[i]);
    }
}

void Ghi_data_Ngay( float y[], int num1){
    int i, j;
    FILE *file = fopen ("C:\\Users\\HP\\Desktop\\doanhthu.txt", "a");
    for (i = 0; i < num1; i++){
        fprintf(file, "%f ", y[i]);
    }
    fclose(file);
}

void read_file_input(FILE *file, float input[], int num){
    int i;
    for (i = 0; i < num; i++){
        fscanf(file, "%f", &input[i]);
    }
}

 void read_file_weight(FILE *file, float w[][100], int num1, int num2){
     int i, j;
     for (i = 0; i < num1; i++){
         for (j = 0; j < num2; j++){
             fscanf(file, "%f", &w[i][j]);
         }
     }
 }

void chuyen_input(float input1[], float input2[], int num){
    int i;
    for (i = 0; i < num; i++){
        if (i >= num - 2 ){
            input1[i] = input2[i];
        }
    }
}
