/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __DEBUG_H__
#define __DEBUG_H_

#include "symtab.h"

 // --- Ho Tro Go Roi ---

void printType(Type* type);									// --- In Thong Tin Kieu --- 
void printConstantValue(ConstantValue* value);				// --- In Gia Tri Hang ---
void printObject(Object* obj, int indent);					// --- In Thong Tin Doi Tuong ---
void printObjectList(ObjectNode* objList, int indent);		// --- In Danh Sach Doi Tuong ---
void printScope(Scope* scope, int indent);					// --- In Thong Tin Khoi (Block) ---

#endif