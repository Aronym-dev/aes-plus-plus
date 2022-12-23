#include <iostream>

void reverseArray(int *arr, int i, int f)
{
    while (i < f) {
        int temp = arr[i];
        arr[i] = arr[f];
        arr[f] = temp;
        i++;
        f--;
    }
}

void leftRotate(int *arr, int start, int end)
{
    reverseArray(arr, 0, start - 1);
    reverseArray(arr, start, end - 1);
    reverseArray(arr, 0, end - 1);
}

int main()
{
    int arr[] = { 1, 2, 3, 4};
    leftRotate(arr, 0, 4);
    for (int i = 0; i < 4; i++) {
        std::cout << arr[i] << std::endl;
    }
}