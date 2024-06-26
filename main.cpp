#include <iostream>
#include <fstream> 
#include <string> 
using namespace std;

template <typename T>
class Matrix{  
private: 
    int lines, columns;
    T** matrix;

    //Удаление матрицы
    void clear(){
        if (matrix != nullptr) {
            for (int i=0; i<lines; i++) {
                delete[] matrix[i];
            }
        delete[] matrix;
        }
    }

    //Функция для нахождения дополнений
    double addition(int stroka, int stolb) {
        Matrix temp(lines-1,columns-1); 
        int m_l,m_c;
        m_l=0;
        for (int t_l=0; t_l<lines-1; t_l++) {
            if (t_l==stroka-1) {
                m_l=1;
            }
            m_c=0;
            for (int t_c=0; t_c<lines-1; t_c++) {
                if (t_c == stolb-1) {
                    m_c = 1;
                }
                temp.matrix[t_l][t_c] = matrix[t_l+m_l][t_c+m_c];
            }
        }
        int one;
        if ((stroka+stolb)%2==0) {
            one = 1;
        } else {
            one = -1;
        }
        return one*temp.determinant();
    }
public: 
    //Конструктор по умолчанию(пустая матрица)
    Matrix() {
        lines = 0;
        columns = 0;
        matrix = nullptr;
    } 
    //Конструктор матрицы с заданными размерами
    Matrix(int k_lines, int k_columns) {
        lines = k_lines;
        columns = k_columns;
        matrix = new T* [lines]; //выделение динамической памяти под массив указателей
        for (int i=0; i<lines; i++) {
             matrix[i] = new T [columns]; //выделение динамической памяти для массива значений
        }
    }
    //Конструктор для считывания матрицы из файла
    Matrix(ifstream& in) {
        in >> lines;
        in >> columns;
        matrix = new T* [lines];
        for (int i=0; i<lines; i++) {
            matrix[i] = new T [columns];
            for (int j=0; j<columns; j++) {
                in >> matrix[i][j];
            }
        }
    }
    //Конструктор копирования
    Matrix(const Matrix & M) {
        lines = M.lines;
        columns = M.columns;
        matrix = new T* [lines];
        T** mx = matrix;
        for (int i=0; i<lines; i++) {
             matrix[i] = new T [columns];
        }
        for (int i=0; i<lines; i++){
            for (int j=0; j<columns; j++) {
                mx[i][j]=M.matrix[i][j];
            }
        }

    }
    //Деструктор
    ~Matrix() {
        clear();
    }

    //Ввод с консоли >>
    friend istream& operator>>(istream& in, Matrix& M) {
        cout << "Ввод матрицы" << endl;
        if (M.lines == 0 || M.columns == 0) {
            cout << "Введите количество строк: ";
            in >> M.lines;
            cout << "Введите количество столбцов: ";
            in >> M.columns;
        }
        M.matrix = new T* [M.lines];
        for (int i=0; i<M.lines; i++) {
            M.matrix[i] = new T [M.columns];
            for (int j=0; j<M.columns; j++) {
                cout << "Введите число: строка - " << i+1 << "; столбец - " << j+1 << ": ";
                in >> M.matrix[i][j];
            }
        }
        cout << endl;
        return in;
    }
    //Вывод в консоль <<
    friend ostream& operator<<(ostream& out, const Matrix& M) {
        for (int i=0; i<M.lines; i++) {
            for (int j=0; j<M.columns; j++) {
                out << M.matrix[i][j] << " ";
            }
            out << endl;
        }
        out << endl;
        return out;
    }
    //Ввод с файла >>
    friend ifstream& operator>>(ifstream& infile, Matrix& M) {
        infile >> M.lines;
        infile >> M.columns;
        M.matrix = new T* [M.lines];
        for (int i=0; i<M.lines; i++) {
            M.matrix[i] = new T [M.columns];
            for (int j=0; j<M.columns; j++) {
                infile >> M.matrix[i][j];
            }
        }
        return infile;
    }
    //Вывод в файл <<
    friend ofstream& operator<<(ofstream& outfile, const Matrix& M) {
        for (int i=0; i<M.lines; i++) {
            for (int j=0; j<M.columns; j++) {
                outfile << M.matrix[i][j] << " ";
            }
            outfile << endl;
        }
        return outfile;
    }

    //перегрузка операторов + и -
    Matrix operator +(const Matrix & second) {
        if (this->lines == second.lines && this->columns == second.columns) {
            cout << "Матрица сложения:" << endl;
            Matrix M(lines,columns);
            for (int i=0; i<lines; i++) {
                for (int j=0; j<columns; j++) {
                    M.matrix[i][j] = this->matrix[i][j] + second.matrix[i][j];
                }
            }
            return M;
        } 
        throw "Матрицы разного порядка, сложить нельзя!"; //Оператор throw генерирует исключение, которое представлено временным объектом типа PlusError
    }
    Matrix operator -(const Matrix & second) {
        if (this->lines == second.lines && this->columns == second.columns) {
            cout << "Матрица вычитания:" << endl;
            Matrix M(lines,columns);
            for (int i=0; i<lines; i++) {
                for (int j=0; j<columns; j++) {
                    M.matrix[i][j] = this->matrix[i][j] - second.matrix[i][j];
                }
            }
            return M;
        } 
        throw "Матрицы разного порядка, вычесть нельзя!";
    }

    //перегрузка оператора * для умножения матриц и умножения матрицы на скаляр
    Matrix operator *(const Matrix & second) {
        if (this->columns == second.lines) {
            cout << "Матрица произведения:" << endl;
            Matrix M(lines,second.columns);
            for (int i=0; i<lines; i++) {
                for (int j=0; j<second.columns; j++) {
                    M.matrix[i][j] = 0;
                    for (int k=0; k<columns; k++) {
                        M.matrix[i][j] += this->matrix[i][k] * second.matrix[k][j];
                    }
                }
            }
            return M;
        }
        throw "Кол-во столбцов 1-й матрицы не равно кол-ву строк 2-й матрицы, умножить нельзя!";
    } 
    Matrix operator *(T scalar) {
        Matrix M(lines,columns);
        for (int i=0; i<lines; i++) {
            for (int j=0; j<columns; j++) {
                M.matrix[i][j] = this->matrix[i][j] * scalar;
            }
        }
        return M;
    }

    //Функция для нахождения детерминанта
    double determinant() {
        double det=0;
        int one;
        T ** M = this->matrix;
        if (this->lines != this->columns) {
           throw "Матрица не квадратная, нет детерминанта!";
        } else {
            if (lines == 1) { 
                return matrix[0][0];
            } else if (lines == 2) { 
                return (matrix[0][0]*matrix[1][1] - matrix[0][1]*matrix[1][0]);
            } else {
                det = 0;
                for (int i=0; i<lines; i++) {
                    Matrix temp(lines-1,columns-1);
                    for (int j=1; j<lines; j++) {
                        int t = 0;
                        for (int k=0; k<lines; k++) {
                            if (k == i) {
                                continue;
                            }
                            temp.matrix[j-1][t] = matrix[j][k];
                            t++;
                        }
                    }
                    if ((i+1+1)%2==0) {
                        one = 1;
                    } else {
                        one = -1;
                    }
                    det += one*matrix[0][i]*temp.determinant();
                }
            }
        }
        return det;
    }
    //Функция для нахождения присоединенной матрицы - A*
    Matrix join() {
        T ** M = this->matrix;
        Matrix temp(lines,columns);
        for (int i=0; i<lines; i++) {
            for (int j=0; j<columns; j++){
                temp.matrix[i][j] = matrix[i][j]; 
            }
        }
        Matrix m(lines,columns);
        if (lines != columns) {
            throw "Матрица не квадратная, нет присоединенной!";
        } else {
            for (int i=0; i<lines; i++) {
                for (int j=0; j<columns; j++){
                    m.matrix[i][j] = temp.addition(i+1,j+1);
                }
            }
        } 
        return m;
    }
    //Функция для транспонирования матрицы
    void transposed() {
        T** M = new T*[columns];
        for (int i=0; i<columns; i++) {
            M[i] = new T[lines];
            for(int j=0; j<lines; j++) {
                M[i][j] = matrix[j][i];
            }
        }
        clear();
        int l = lines;
        lines = columns;
        columns = l;
        matrix = M;
    }
    //Перегрузка оператра ! для вычисления обраной матрицы
    Matrix operator !() {
        Matrix temp(lines,columns);
        for (int i=0; i<lines; i++) {
            for (int j=0; j<lines; j++) {
                temp.matrix[i][j] = matrix[i][j];
            }
        }
        if (lines != columns) {
            throw "Матрица не квадратная, обратной нет!";
        } else {
            double det = temp.determinant();
            if (det == 0) {
                throw "Нулевой детерминант, обратной матрицы нет!";
            } else {
                Matrix temp_2 = temp.join();
                temp_2.transposed();
                Matrix temp_3 = temp_2 * (1/det);
                cout << "Обратная матрица:" << endl;
                return temp_3;
            }
        } 
    }

    //Перегрузка оператора присваивания =
    Matrix& operator =(const Matrix & second) {
        clear();
        lines = second.lines;
        columns=second.columns;
        matrix = new T * [lines];
        for (int i=0; i<lines; i++) {
            matrix[i] = new T [columns];
            for (int j=0; j<columns; j++) {
                matrix[i][j] = second.matrix[i][j];
            }
        }
        return *this;
    }

    //Cтатические методы создания нулевых и единичных матриц указанного размера
    static Matrix<T> zero(int k_lines, int k_columns) {
        Matrix<T> M(k_lines, k_columns);
        for (int i = 0; i < k_lines; i++) {
            for (int j = 0; j < k_columns; j++) {
                M.matrix[i][j] = 0;
            }
        }
        return M;
    }
    static Matrix<T> id(int k_lines, int k_columns) {
        if (k_lines == k_columns) {
            Matrix<T> M(k_lines, k_columns);
            for (int i = 0; i < k_lines; i++) {
                for (int j = 0; j < k_columns; j++) {
                    if (i!=j) {
                        M.matrix[i][j] = 0;
                    } else {
                        M.matrix[i][j] = 1;
                    } 
                }
            }
            return M;
        }
        throw "Матрица не квадратная, она не может быть единичной!";
    }
     
    //Перегрузка операторов == и != для сравнения матриц
    bool operator ==(const Matrix & second){
        if (this->lines == second.lines && this->columns == second.columns) {
            for (int i=0; i<lines; i++) {
                for (int j=0; j<columns; j++) {
                    if (this->matrix[i][j] != second.matrix[i][j]) {
                        cout << "Матрицы не равны" << endl;;
                        return false;
                    }
                } 
            }
            cout <<  "Матрицы равны" << endl;
            return true;
        } else {
            cout << "Матрицы не равны" << endl;
            return false;
        }
    }
    bool operator !=(const Matrix & second){
        return !(*this == second);
    }
    //Перегрузка операторов == и != для сравнения матрицы и скаляра 
    bool operator ==(double scalar) {
        for (int i=0; i<lines; i++) {
            for(int j=0; j<columns; j++) {
                if (i!=j && matrix[i][j] != 0) {
                    cout << "Матрица не равна скаляру" << endl;
                    return false;
                } 
                if (i==j && matrix[i][j] != scalar) {
                    cout << "Матрица не равна скаляру" << endl;
                    return false;
                }
            }
        }
        cout << "Матрица равна скаляру" << endl;
        return true;
    }
    bool operator !=(double scalar) {
        return !(*this == scalar);
    }

    //Функции для сложения и вычитания строк или столбцов (если thing = 0 - строка, если thing = 1 - столбец)
    void plus(int thing, int one, int two, double L) {
        if (thing == 0) {
            for (int i=0; i<columns; i++) {
                matrix[one-1][i] += matrix[two-1][i] * L;
            }
        } else {
            for (int i=0; i<lines; i++) {
                matrix[i][one-1] += matrix[i][two-1] * L;
            }
        }
    }    
    void minus(int thing, int one, int two, double L) {
        if (thing == 0) {
            for (int i=0; i<columns; i++) {
                matrix[one-1][i] -= matrix[two-1][i] * L;
            }
        } else {
            for (int i=0; i<lines; i++) {
                matrix[i][one-1] -= matrix[i][two-1] * L;
            }
        }
    }
    //Функция для преобразования отдельной строки или столбца
    void multiply(int thing, int one, double L) {
        if (thing == 0) {
            for (int i=0; i<columns; i++) {
                matrix[one-1][i] *= L;
            }
        } else {
            for (int i=0; i<lines; i++) {
                matrix[i][one-1] *= L;
            }
        }
    }
    //Функция для перестановки строки или столбца
    void change(int thing, int one, int two) {
       int temp;
       if (thing == 0) {
            for (int i=0; i<columns; i++) {
                temp = matrix[one-1][i];
                matrix[one-1][i] = matrix[two-1][i];
                matrix[two-1][i] = temp;
            }
        } else {
            for (int i=0; i<lines; i++) {
                temp = matrix[i][one-1];
                matrix[i][one-1] = matrix[i][two-1];
                matrix[i][two-1] = temp;
            }
        } 
    }
};

int main() {
    //1-2
    cout << "№1-2" << endl;
    Matrix<int> A_1;
    cin >> A_1;
    cout << "Матрица ввода с размером А_1:" << endl << A_1;

    Matrix<int> A_2(3,4);
    cin >> A_2;
    cout << "Матрица ввода без размеров А_2:" << endl << A_2; 

    ifstream file_B("matrix_B.txt");
    Matrix<int> B(file_B);  
    file_B.close();
    cout << "Матрица из файла B:" << endl << B;

    ifstream file_C("matrix_C.txt");
    Matrix<double> C;
    file_C >> C;
    ofstream write("write_matrix.txt");
    write << "Матрица из одного файла в другой С:" << endl << C;
    file_C.close();
    write.close();

    //3
    cout << "№3" << endl;
    try {
        Matrix<double> R_1, R_2;
        cin >> R_1 >> R_2;
        try {
            cout << R_1 + R_2;;
        } catch (const char* error_message){//через параметр в блоке catch мы можем получить то сообщение, которое передается оператору throw
            cout << error_message << endl;
        }
        try {
            cout << R_1 - R_2;
        } catch (const char* error_message){
            cout << error_message << endl;
        }   
    } catch (...) {//является последним блоком catch в цепочке обработки исключений и предназначен для обработки общих ситуаций ошибок или случаев, которые не предусмотрены более конкретными блоками catch
        cout << endl;  
    }

    //4
    cout << "№4" << endl;
    try {
        Matrix<int> P_1, P_2;
        cin >> P_1 >> P_2;
        cout << P_1 * P_2;
    } catch(const char* error_message) {
        cout << error_message << endl;
    }
    Matrix<double> S;
    cin >> S;
    double s;
    cout << "Введите скаляр: ";
    cin >> s;
    cout << "Матрица, умноженная на скаляр:" << endl << S*s;

    //5
    cout << "№5" << endl;
    try {
        Matrix<double> D;
        cin >> D;
        try {
            cout << "Детерминант: " << D.determinant() << endl;
        } catch(const char* error_message) {
            cout << error_message << endl;
        }
        try {
            cout << "Присоединенная матрица:" << endl << D.join();
        } catch(const char* error_message) {
            cout << error_message << endl;
        }
        try {
            D.transposed();
            cout << "Транспонированная матрица:" << endl << D;
        } catch(const char* error_message) {
            cout << error_message << endl;
        }
        try {
            D.transposed();
            cout << !D;
        } catch(const char* error_message) {
            cout << error_message << endl;
        }
    } catch(...) {
        cout << endl;
    }

    //6
    cout << "№6" << endl;
    Matrix<int> X, Y;
    cin >> X >> Y;
    cout << "X:" << endl << X;
    X = Y;
    cout << "Y:" << endl << Y << "X:" << endl << X;

    //7
    cout << "№7" << endl;
    cout << "Нулевая матрица:" << endl << Matrix<int>::zero(3,4);
    try {
        Matrix<int> E_1 = Matrix<int>::id(3,4);
        cout << "Единичная матрица:" << endl << E_1;
    } catch(const char* error_message) {
        cout << error_message << endl;
    }
    try {
        Matrix<int> E_2 = Matrix<int>::id(4,4);
        cout << "Единичная матрица:" << endl << E_2;
    } catch(const char* error_message) {
        cout << error_message << endl;
    }
}