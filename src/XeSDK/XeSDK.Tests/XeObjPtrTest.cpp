#include "pch.h"
#include <XeSDK/XeObjPtr.h>
#include "gtest/gtest.h"

struct XeObjPtrFoo : public Xe::IObject
{
	int GetRefsCount()
	{
		AddRef();
		return Release();
	}
};

TEST(XeObjPtrTest, NullCheck)
{
	Xe::ObjPtr<XeObjPtrFoo> foo;
	EXPECT_EQ(nullptr, foo.Get());
	EXPECT_EQ(nullptr, (XeObjPtrFoo*)foo);
	EXPECT_TRUE(!foo);
}

TEST(XeObjPtrTest, PointerCheck)
{
	XeObjPtrFoo* myfoo = new XeObjPtrFoo;
	Xe::ObjPtr<XeObjPtrFoo> foo(myfoo);

	EXPECT_EQ(myfoo, foo.Get());
	EXPECT_EQ(myfoo, *&foo);
	EXPECT_EQ(myfoo, (XeObjPtrFoo*)foo);
	EXPECT_EQ(myfoo->GetRefsCount(), foo->GetRefsCount());
	EXPECT_TRUE(myfoo == foo);
	EXPECT_FALSE(myfoo != foo);
	EXPECT_TRUE(foo);
	EXPECT_FALSE(!foo);
}

TEST(XeObjPtrTest, RefsCheck)
{
	XeObjPtrFoo* myfoo = new XeObjPtrFoo;
	myfoo->AddRef();
	EXPECT_EQ(2, myfoo->GetRefsCount());

	// ObjPtr has to remove a reference when holds an acutal IObject
	EXPECT_EQ(2, Xe::ObjPtr<XeObjPtrFoo>(myfoo)->GetRefsCount());
	EXPECT_EQ(1, myfoo->GetRefsCount());

	// ObjPtr must not increment the references count when created from an IObject
	Xe::ObjPtr<XeObjPtrFoo> foo = myfoo;
	EXPECT_EQ(1, foo->GetRefsCount());
	myfoo->AddRef();

	// ObjPtr has to increment the references count when created from another ObjPtr
	EXPECT_EQ(3, Xe::ObjPtr<XeObjPtrFoo>(foo)->GetRefsCount());
	EXPECT_EQ(2, foo->GetRefsCount());

	// ObjPtr cannot remove a reference when holds a null pointer
	*&Xe::ObjPtr<XeObjPtrFoo>(myfoo) = nullptr;
	EXPECT_EQ(2, foo->GetRefsCount());
}