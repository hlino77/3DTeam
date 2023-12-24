#pragma once


struct tagTypeLess    // Interface
{
	virtual void* GetValue() PURE;
};

template<typename T>
struct tagTypeLessData : public tagTypeLess
{
public:
	tagTypeLessData() = default;
	tagTypeLessData(T _value) :value(_value) {}
	virtual void* GetValue() override { return &value; }
	void            SetValue(T _value) { value = _value; }

private:
	T value;
};

template<typename T>
struct tagTypeLessData<T*> : public tagTypeLess
{
public:
	tagTypeLessData() = default;
	tagTypeLessData(T* _value) :value(_value) {}
	virtual void* GetValue() override { return value; } // �̰� ĳ���� �ȵǸ� value �ٷ� ����������
	void            SetValue(T* _value) { value = _value; }

private:
	T* value;
};
