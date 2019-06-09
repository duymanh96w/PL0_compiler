        Chương trình biên dịch ngôn ngữ PL/0
1. Biên dịch:
    gcc main.c

2. Tham số dòng lệnh:
    -s: in kết quả hợp ngữ ra file với tên tương tự file đầu vào, phần mở rộng là .asm
    -b: in mã thực thi ra file với tên tương tự file đầu vào, phần mở rộng là .bin
    -r: thực thi mã lệnh của chương trình

3. Cách chạy
    output_file_name [tham số] input_test_file
    File test cần nằm cùng thư mục
    Trong trường hợp không sử dụng tham số, chương trình chỉ kiểm tra cú pháp, ngữ nghĩa và in kết quả ra console
    Chỉ có thể dùng 1 tham số hoặc 3 tham số cùng lúc, không thẻ sử dụng 2 tham số
    Trong trường hợp sử dụng 3 tham số cùng lúc cần nhập theo thứ tự -s, -b, -r, nếu nhập sai sẽ báo lỗi

VD: ./a.out -s test1.pl0
    ./a.out -r test1.pl0
