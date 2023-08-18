#include <iostream>

#define WIN32_LEAN_AND_MEAN
#define WIN32_NO_STATUS
#define NOMINMAX
#include <Windows.h>
#include <winternl.h>

volatile float g_value = -1.0f;

float Q_fabsf(float f) {
	int tmp = *(int*)&f;
	tmp &= 0x7FFFFFFF;
	return *(float*)&tmp;
}

__declspec(noinline)
float func1()
{
	float result = 0.0f;
	for (size_t i = 0; i < 100'000'000'00; i++)
	{
		result += Q_fabsf(g_value);
	}
	return result;
}

__declspec(noinline)
float func2()
{
	float result = 0.0f;
	for (size_t i = 0; i < 100'000'000'00; i++)
	{
		result += fabsf(g_value);//will inline intrin
	}
	return result;
}

int main()
{
	ULONGLONG TickCount64_start_1 = GetTickCount64();
	func1();
	ULONGLONG TickCount64_end_1 = GetTickCount64();

	ULONGLONG TickCount64_start_2 = GetTickCount64();
	func2();//faster
	ULONGLONG TickCount64_end_2 = GetTickCount64();

	printf("%llu\n", TickCount64_end_1 - TickCount64_start_1);//3812
	printf("%llu\n", TickCount64_end_2 - TickCount64_start_2);//3782

	return 0;
}

