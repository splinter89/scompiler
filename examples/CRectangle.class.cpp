class CRectangle {
    private:
        int x, y;
    public:
        int area;
        void set_values (int, int);
        int calc_area() { area = x * y; return area; }
};

void CRectangle::set_values(int a, int b) {
    x = a;
    y = b;
}

int main() {
    CRectangle rect;
    rect.set_values(5, 10);
    int calculated_area = rect.calc_area(); // 50
    int double_area = rect.area * 2; // 100
    return 0;
}
