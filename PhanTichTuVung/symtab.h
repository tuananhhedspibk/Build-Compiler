/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __SYMTAB_H__
#define __SYMTAB_H__

#define MAX_IDENT_LEN 15

// --- Cau Truc Cua Cac Thanh Phan Trong Bang Ki Hieu ---

enum TypeClass {						// --- Phan Loai Kieu ---
	TP_INT,
	TP_CHAR,
	TP_ARRAY
};

enum ObjectKind {						// --- Cac Kieu Doi Tuong ---
	OBJ_CONSTANT,
	OBJ_VARIABLE,
	OBJ_TYPE,
	OBJ_FUNCTION,
	OBJ_PROCEDURE,
	OBJ_PARAMETER,
	OBJ_PROGRAM
};

enum ParamKind {						// --- Cac Kieu Tham So ---
	PARAM_VALUE,						// --- Tham Tri ---
	PARAM_REFERENCE						// --- Tham Chieu ---
};

struct Type_ {							// --- Cau Truc Kieu ---
	enum TypeClass typeClass;
	int arraySize;						// --- Chi Dung Cho Mang ---
	struct Type_ *elementType;
};
typedef struct Type_ Type;				// --- Chuyen Type_ Thanh Type ---
typedef struct Type_ BasicType;			// --- Chuyen Type_ Thanh BasicType ---


struct ConstantValue_ {					// --- Dinh Nghia Kieu Hang ---
	enum TypeClass type;
	union {
		int intValue;
		char charValue;
	};
};
typedef struct ConstantValue_ ConstantValue;

struct Scope_;
struct ObjectNode_;
struct Object_;

//	--- Dinh Nghia Cac Loai Thuoc Tinh ---

struct ConstantAttributes_ {
	ConstantValue* value;
};

struct VariableAttributes_ {
	Type *type;
	struct Scope_ *scope;
};

struct TypeAttributes_ {
	Type *actualType;
};

struct ProcedureAttributes_ {
	struct ObjectNode_ *paramList;
	struct Scope_* scope;
};

struct FunctionAttributes_ {
	struct ObjectNode_ *paramList;
	Type* returnType;
	struct Scope_ *scope;
};

struct ProgramAttributes_ {
	struct Scope_ *scope;
};

struct ParameterAttributes_ {
	enum ParamKind kind;
	Type* type;
	struct Object_ *function;
};

typedef struct ConstantAttributes_ ConstantAttributes;
typedef struct TypeAttributes_ TypeAttributes;
typedef struct VariableAttributes_ VariableAttributes;
typedef struct FunctionAttributes_ FunctionAttributes;
typedef struct ProcedureAttributes_ ProcedureAttributes;
typedef struct ProgramAttributes_ ProgramAttributes;
typedef struct ParameterAttributes_ ParameterAttributes;

//	--- Dinh Nghia Cac Loai Thuoc Tinh ---

struct Object_ {						// --- Cau Truc 1 Doi Tuong ---
	char name[MAX_IDENT_LEN];			// --- Ten Doi Tuong ---
	enum ObjectKind kind;				// --- Loai Doi Truong ---
	union {								// --- Thuoc Tinh Cua Doi Tuong Tren Bang Ki Hieu ---
		ConstantAttributes* constAttrs;
		VariableAttributes* varAttrs;
		TypeAttributes* typeAttrs;
		FunctionAttributes* funcAttrs;
		ProcedureAttributes* procAttrs;
		ProgramAttributes* progAttrs;
		ParameterAttributes* paramAttrs;
	};									// --- Chi Lua Chon 1 Trong So Tren ---
};
typedef struct Object_ Object;

struct ObjectNode_ {						// --- Cau Truc 1 Node Trong Object List---
	Object *object;							// --- Xay Dung Theo Kieu Danh Sach Lien Ket (Linked List) ---
	struct ObjectNode_ *next;
};
typedef struct ObjectNode_ ObjectNode;

struct Scope_ {								// --- Pham Vi (Cau Truc) 1 Block ---
	ObjectNode *objList;					// --- Danh Sach Cac Doi Tuong Trong 1 Block ---
	Object *owner;							// --- Ham, Thu Tuc, Chuong Trinh Tuong Ung Voi Block ---
	struct Scope_ *outer;					// --- Pham Vi Bao Ngoai ---	
};
typedef struct Scope_ Scope;				// --- Chuyen Scope_ Thanh Scope ---

struct SymTab_ {							// --- Cau Truc Cua 1 Bang Ki Hieu ---
	Object* program;						// --- Chuong Trinh Chinh ---
	Scope* currentScope;					// --- Pham Vi Hien Tai , Dung De Ghi Nho Block Hien Dang Duyet ---
	ObjectNode *globalObjectList;			// --- Cac Doi Tuong Toan Cuc : WRITEI, WRITEC, WRITELN, READI, READC ---
};
typedef struct SymTab_ SymTab;				// --- Chuyen SymTab_ Thanh Symtab , Sau Nay Khi Dung SymTab Thi Khong Phai Struct SymTab Nua ---

// --- Danh Sach Cac Ham Trong symtab.c ---

Type* makeIntType(void);
Type* makeCharType(void);
Type* makeArrayType(int arraySize, Type* elementType);
Type* duplicateType(Type* type);
int compareType(Type* type1, Type* type2);
void freeType(Type* type);

// --- Cac Ham Tao Hang So ---

ConstantValue* makeIntConstant(int i);
ConstantValue* makeCharConstant(char ch);
ConstantValue* duplicateConstantValue(ConstantValue* v);

// --- Cac Ham Tao Hang So ---

Scope* createScope(Object* owner, Scope* outer);

// --- Cac Ham Tao Doi Tuong ---

Object* createProgramObject(char *programName);
Object* createConstantObject(char *name);
Object* createTypeObject(char *name);
Object* createVariableObject(char *name);
Object* createFunctionObject(char *name);
Object* createProcedureObject(char *name);
Object* createParameterObject(char *name, enum ParamKind kind, Object* owner);

// --- Cac Ham Tao Doi Tuong ---

Object* findObject(ObjectNode *objList, char *name);

void initSymTab(void);
void cleanSymTab(void);
void enterBlock(Scope* scope);
void exitBlock(void);
void declareObject(Object* obj);

#endif