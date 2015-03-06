#include "typecheck.hpp"

// Defines the function used to throw type errors. The possible
// type errors are defined as an enumeration in the header file.
void typeError(TypeErrorCode code) {
  switch (code) {
    case undefined_variable:
      std::cerr << "Undefined variable." << std::endl;
      break;
    case undefined_method:
      std::cerr << "Method does not exist." << std::endl;
      break;
    case undefined_class:
      std::cerr << "Class does not exist." << std::endl;
      break;
    case undefined_member:
      std::cerr << "Class member does not exist." << std::endl;
      break;
    case not_object:
      std::cerr << "Variable is not an object." << std::endl;
      break;
    case expression_type_mismatch:
      std::cerr << "Expression types do not match." << std::endl;
      break;
    case argument_number_mismatch:
      std::cerr << "Method called with incorrect number of arguments." << std::endl;
      break;
    case argument_type_mismatch:
      std::cerr << "Method called with argument of incorrect type." << std::endl;
      break;
    case while_predicate_type_mismatch:
      std::cerr << "Predicate of while loop is not boolean." << std::endl;
      break;
    case if_predicate_type_mismatch:
      std::cerr << "Predicate of if statement is not boolean." << std::endl;
      break;
    case assignment_type_mismatch:
      std::cerr << "Left and right hand sides of assignment types mismatch." << std::endl;
      break;
    case return_type_mismatch:
      std::cerr << "Return statement type does not match declared return type." << std::endl;
      break;
    case constructor_returns_type:
      std::cerr << "Class constructor returns a value." << std::endl;
      break;
    case no_main_class:
      std::cerr << "The \"Main\" class was not found." << std::endl;
      break;
    case main_class_members_present:
      std::cerr << "The \"Main\" class has members." << std::endl;
      break;
    case no_main_method:
      std::cerr << "The \"Main\" class does not have a \"main\" method." << std::endl;
      break;
    case main_method_incorrect_signature:
      std::cerr << "The \"main\" method of the \"Main\" class has an incorrect signature." << std::endl;
      break;
  }
  exit(1);
}

bool inClass;
int temp;

// TypeCheck Visitor Functions: These are the functions you will
// complete to build the symbol table and type check the program.
// Not all functions must have code, many may be left empty.

void TypeCheck::visitProgramNode(ProgramNode* node) {
  // WRITEME: Replace with code if necessary
  classTable = new ClassTable;
  currentLocalOffset = 0;
  currentParameterOffset = 8;
  currentMemberOffset = -4;
  node->visit_children(this);
  
  // Error: whether Main class has a main method
  std::map<std::basic_string<char>, ClassInfo>::iterator c_iter = classTable->find("Main");
  if(c_iter != classTable->end()){
    ClassInfo c_info = c_iter->second;
    if(c_info.membersSize == 0){
        MethodTable* m_table = c_info.methods;
        std::map<std::basic_string<char>, MethodInfo>::iterator m_iter = m_table->find("main");
        if(m_iter == m_table->end()){
            // Error: no main method
            typeError(no_main_method);
        }
        else{
            MethodInfo m_info = m_iter->second;
            CompoundType returnType = m_info.returnType;
            if(returnType.baseType != bt_none){
                // Error: "main" method has incorrect signature (doesn't return bt_none)
                typeError(main_method_incorrect_signature);
            }
        }
    }
    else{   
        // Error: Main Class has members NOT DONE!!
        typeError(main_class_members_present);
    }
  }
  else{
    // Error: no Main Class
    typeError(no_main_class);
  }
}

void TypeCheck::visitClassNode(ClassNode* node) {
  // WRITEME: Replace with code if necessary
  //std::cout << "in class\n";
  inClass = true;
  currentLocalOffset = 0;
  currentParameterOffset = 8;
  currentMemberOffset = -4;
  
  ClassInfo* classInfo = new ClassInfo;

  if(node->identifier_2 != NULL)
    classInfo->superClassName = node->identifier_2->name;
  else
    classInfo->superClassName = "";

  VariableTable* varTable = new VariableTable;
  MethodTable* methTable = new MethodTable;

  classInfo->members = varTable;
  classInfo->methods = methTable;

  currentVariableTable = varTable;
  currentMethodTable = methTable;  
  
  //  int* memberSize = &(classInfo->membersSize);
  
  (*classTable)[node->identifier_1->name] = (*classInfo);
  node->visit_children(this);

  (*classTable)[node->identifier_1->name].membersSize = (*classTable)[node->identifier_1->name].members->size() * 4;  
}

void TypeCheck::visitMethodNode(MethodNode* node) {
  // WRITEME: Replace with code if necessary
  inClass = false;
  currentLocalOffset = 0;
  currentParameterOffset = 8;

  //std::cout << "in method\n";
  std::list<CompoundType> *param = new std::list<CompoundType>();
  std::list<ParameterNode*>::iterator parameter_iter;
  MethodInfo* methInfo = new MethodInfo;


  VariableTable* variableTable = new VariableTable;
  currentVariableTable = variableTable;
  methInfo->variables = variableTable;

  node->visit_children(this);

  if(node->parameter_list != NULL){
    // Initiate Compound List for parameters
    for(parameter_iter = node->parameter_list->begin(); parameter_iter != node->parameter_list->end(); ++parameter_iter){

      CompoundType* cType = new CompoundType;
      cType->baseType = (*parameter_iter)->basetype;
      cType->objectClassName = (*parameter_iter)->identifier->name;

      param->push_back((*cType));
    }
    methInfo->parameters = param;
    methInfo->localsSize = (currentVariableTable->size() - node->parameter_list->size()) * 4;
  }
  else{
    methInfo->localsSize = currentVariableTable->size() * 4;
  }

  CompoundType* type = new CompoundType;
  type->baseType = node->type->basetype;
  type->objectClassName = node->type->objectClassName;
  methInfo->returnType = (*type);

  (*currentMethodTable)[node->identifier->name] = (*methInfo);
  inClass = true;
}

void TypeCheck::visitMethodBodyNode(MethodBodyNode* node) {
  // WRITEME: Replace with code if necessary
  node->visit_children(this);

}

void TypeCheck::visitParameterNode(ParameterNode* node) {
  // WRITEME: Replace with code if necessary
  node->visit_children(this);
  std:: cout << "in params ";
  
  CompoundType* comType = new CompoundType;
  comType->baseType = node->type->basetype;
  comType->objectClassName = node->type->objectClassName;
  
  std:: cout << node->identifier->name << " ";
  node->identifier->objectClassName = node->type->objectClassName;
  node->identifier->basetype = node->type->basetype;
  //std::cout << node->identifier->objectClassName << "\n";
  
  currentParameterOffset = currentParameterOffset + 4;
  
  VariableInfo varInfo = {(*comType),currentParameterOffset, 4};
  (*currentVariableTable)[node->identifier->name] = varInfo;
}

void TypeCheck::visitDeclarationNode(DeclarationNode* node) {
  // WRITEME: Replace with code if necessary
  std::list<IdentifierNode*>::iterator id_iter;
  //std::cout << "in decls\n";
  node->visit_children(this);

  if(node->identifier_list != NULL){
    for(id_iter = node->identifier_list->begin(); id_iter != node->identifier_list->end(); ++id_iter){
      CompoundType* compoundType = new CompoundType;
      compoundType->baseType = node->type->basetype;
      compoundType->objectClassName = node->type->objectClassName;


      // (*id_iter)->basetype = node->type->basetype;
      // (*id_iter)->objectClassName = node->type->objectClassName;

      // if((*id_iter)->basetype == bt_boolean)
      //   std::cout << (*id_iter)->name << " bool\n";
      // if((*id_iter)->basetype == bt_integer)
      //   std::cout << (*id_iter)->name << " int\n";
      // if((*id_iter)->basetype == bt_object)
      //   std::cout << (*id_iter)->name << " object\n";

      VariableInfo* varInfo = new VariableInfo;
      varInfo->type = (*compoundType);
      if(inClass == true){
        currentMemberOffset = currentMemberOffset + 4;
        varInfo->offset = currentMemberOffset;
      }
      else{
        currentLocalOffset = currentLocalOffset - 4;
        varInfo->offset = currentLocalOffset;
      }
      varInfo->size = 4;
      (*currentVariableTable)[(*id_iter)->name] = (*varInfo);
    }
  }
}

void TypeCheck::visitReturnStatementNode(ReturnStatementNode* node) {
  // WRITEME: Replace with code if necessary
  node->visit_children(this);
  
}

void TypeCheck::visitAssignmentNode(AssignmentNode* node) {
  // WRITEME: Replace with code if necessary
  node->visit_children(this);

}

void TypeCheck::visitCallNode(CallNode* node) {
  // WRITEME: Replace with code if necessary
    node->visit_children(this);

}

void TypeCheck::visitIfElseNode(IfElseNode* node) {
  // WRITEME: Replace with code if necessary
    node->visit_children(this);

}

void TypeCheck::visitWhileNode(WhileNode* node) {
  // WRITEME: Replace with code if necessary
    node->visit_children(this);

}

void TypeCheck::visitPrintNode(PrintNode* node) {
  // WRITEME: Replace with code if necessary
    node->visit_children(this);
    std::cout << "print\n";
}

void TypeCheck::visitPlusNode(PlusNode* node) {
  // WRITEME: Replace with code if necessary
  node->visit_children(this);
  std::cout << "plus\n";
  if(node->expression_1->basetype != bt_integer){
    typeError(expression_type_mismatch);
  }
  else{
    if(node->expression_2->basetype != bt_integer){
        typeError(expression_type_mismatch);
    }
  }
  
  node->basetype = bt_integer;
}

void TypeCheck::visitMinusNode(MinusNode* node) {
  // WRITEME: Replace with code if necessary
  node->visit_children(this);
    std::cout << "minus\n";
  if(node->expression_1->basetype != bt_integer){
    typeError(expression_type_mismatch);
  }
  else{
    if(node->expression_2->basetype != bt_integer)
    typeError(expression_type_mismatch);
  }
  
  node->basetype = bt_integer;
}

void TypeCheck::visitTimesNode(TimesNode* node) {
  // WRITEME: Replace with code if necessary
  node->visit_children(this); 
  std::cout << "times\n";
  if(node->expression_1->basetype != bt_integer){
    typeError(expression_type_mismatch);
  }
  else{
    if(node->expression_2->basetype != bt_integer)
    typeError(expression_type_mismatch);
  }
  
  node->basetype = bt_integer;
}

void TypeCheck::visitDivideNode(DivideNode* node) {
  // WRITEME: Replace with code if necessary
  node->visit_children(this);
  std::cout << "divide\n";
  if(node->expression_1->basetype != bt_integer){
    typeError(expression_type_mismatch);
  }
  else{
    if(node->expression_2->basetype != bt_integer)
    typeError(expression_type_mismatch);
  }
  
  node->basetype = bt_integer;
}

void TypeCheck::visitLessNode(LessNode* node) {
  // WRITEME: Replace with code if necessary
  node->visit_children(this);
std::cout << "less\n";
  if(node->expression_1->basetype != bt_integer){
    typeError(expression_type_mismatch);
  }
  else{
    if(node->expression_2->basetype != bt_integer)
    typeError(expression_type_mismatch);
  }
  
  node->basetype = bt_boolean;
}

void TypeCheck::visitLessEqualNode(LessEqualNode* node) {
  // WRITEME: Replace with code if necessary
  node->visit_children(this);
std::cout << "lessequal\n";
  if(node->expression_1->basetype != bt_integer){
    typeError(expression_type_mismatch);
  }
  else{
    if(node->expression_2->basetype != bt_integer)
    typeError(expression_type_mismatch);
  }
  
  node->basetype = bt_boolean;
}

void TypeCheck::visitEqualNode(EqualNode* node) {
  // WRITEME: Replace with code if necessary
  node->visit_children(this);
  std::cout << "equal\n";
  
  if(node->expression_1->basetype != node->expression_2->basetype)
    typeError(expression_type_mismatch);

  node->basetype = bt_boolean;
}

void TypeCheck::visitAndNode(AndNode* node) {
  // WRITEME: Replace with code if necessary
  node->visit_children(this);

  if(node->expression_1->basetype != bt_boolean){
      std::cout << "and\n";
    typeError(expression_type_mismatch);
  }
  else{
    if(node->expression_2->basetype != bt_boolean)
        typeError(expression_type_mismatch);
  }
  
  node->basetype = bt_boolean;
}

void TypeCheck::visitOrNode(OrNode* node) {
  // WRITEME: Replace with code if necessary
  node->visit_children(this);
  std::cout << "or\n";

  if(node->expression_2->basetype == bt_boolean)
    std::cout << " bool\n";
  if(node->expression_2->basetype == bt_integer)
    std::cout << " int\n";
  if(node->expression_2->basetype == bt_object)
    std::cout << " object\n";

  if(node->expression_1->basetype != bt_boolean){
    typeError(expression_type_mismatch);
  }
  else{
    if(node->expression_2->basetype != bt_boolean)
    typeError(expression_type_mismatch);
  }
  
  node->basetype = bt_boolean;
}

void TypeCheck::visitNotNode(NotNode* node) {
  // WRITEME: Replace with code if necessary
  node->visit_children(this);
  std::cout << "not\n";

  if(node->expression->basetype != bt_boolean){
    typeError(expression_type_mismatch);
  }
  
  node->basetype = bt_boolean;
}

void TypeCheck::visitNegationNode(NegationNode* node) {
  // WRITEME: Replace with code if necessary
  node->visit_children(this);
  
  std::cout << "negation\n";
  if(node->expression->basetype != bt_integer){
    typeError(expression_type_mismatch);
  }
  
  node->basetype = bt_integer;
}

void TypeCheck::visitMethodCallNode(MethodCallNode* node) {
  // WRITEME: Replace with code if necessary
  node->visit_children(this);
  std::cout << "methodCall\n";

//   std::map<std::basic_string<char>, ClassInfo>::iterator c_iter;
//   std::map<std::basic_string<char>, MethodInfo>::iterator m_iter;
//   std::map<std::basic_string<char>, VariableInfo>::iterator v_iter;
//   std::map<std::basic_string<char>, ClassInfo>::iterator super_c_iter;
//   std::map<std::basic_string<char>, MethodInfo>::iterator super_m_iter;
//   ClassInfo c_info;
//   MethodInfo m_info;
//   VariableInfo v_info;
//   MethodTable* m_table;
//   std::string superName;
//   ClassInfo super_c_info;
//   MethodTable* super_m_table;
//   bool found = false;
//   std::string className;
  
// // CHECKING METHOD
//   m_iter = currentMethodTable->find(node->identifier_1->name);
//   if(m_iter != currentMethodTable->end()){
//   // Method exist in local MethodTable
//     m_info = m_iter->second;
//     if(m_info.parameters->size() == node->expression_list->size()){
//         std::list<CompoundType>::iterator m_param = m_info.parameters->begin();
//         std::list<ExpressionNode*>::iterator n_param = node->expression_list->begin();

//         for(; m_param != m_info.parameters->end() && n_param != node->expression_list->end(); ++m_param, ++n_param){
//             if(m_param->baseType != (*n_param)->basetype)
//             // Error: Parameters dont have the same types
//                 typeError(argument_type_mismatch);
//         }
//     }
//     else{
//       // Error: Wrong number of parameters
//       typeError(argument_number_mismatch);
//     }
//   }
//   else{    
//   // CHECKING VARIABLE  
//     v_iter = currentVariableTable->find(node->identifier_1->name);
//     if(v_iter != currentVariableTable->end()){
//         v_info = v_iter->second;
//         className = v_info.type.objectClassName;
//         c_iter = classTable->find(className);
//         if(c_iter != classTable->end()){
//           c_info = c_iter->second;
//           m_table = c_info.methods;
//           m_iter = m_table->find(node->identifier_2->name);
//           if(m_iter != m_table->end()){
//           // Method found on first try
//             m_info = m_iter->second;
//               if(m_info.parameters->size() == node->expression_list->size()){
//                   std::list<CompoundType>::iterator m_param = m_info.parameters->begin();
//                   std::list<ExpressionNode*>::iterator n_param = node->expression_list->begin();
//                   for(; m_param != m_info.parameters->end() && n_param != node->expression_list->end(); ++m_param, ++n_param){
//                       if(m_param->baseType == bt_boolean)
//                         std::cout << " bool\n";
//                       if(m_param->baseType == bt_integer)
//                         std::cout << " int\n";
//                       if(m_param->baseType == bt_object)
//                         std::cout << " object\n";
//                       if(m_param->objectClassName != "")
//                         std::cout << m_param->objectClassName << "\n";
//                       if(m_param->baseType != (*n_param)->basetype)
//                       // Error: Parameters dont have the same types
//                           typeError(argument_type_mismatch);
//                   }
//               }
//           }
//           else{
//           // Can't find the method in that Object Class's method table
//           // Need to check super classes
//               superName = c_info.superClassName;
//               while (!found){
//                   super_c_iter = classTable->find(superName);
//                   super_c_info = super_c_iter->second;
//                   super_m_table = super_c_info.methods;
//                   super_m_iter = super_m_table->find(node->identifier_2->name);
//                   if(superName == ""){
//                   // Error: Method is not defined
//                       typeError(undefined_method);
//                   }
//                   if(super_m_iter == super_m_table->end()){ // Move to super class
//                       superName = super_c_info.superClassName;
//                   }
//                   else{ // If method does exist
//                       found = true;
//                   }
//               }
//               // To do this with expressions
//               m_info = super_m_iter->second;
//               if(m_info.parameters->size() == node->expression_list->size()){
//                   std::list<CompoundType>::iterator m_param = m_info.parameters->begin();
//                   std::list<ExpressionNode*>::iterator n_param = node->expression_list->begin();
          
//                   for(; m_param != m_info.parameters->end() && n_param != node->expression_list->end(); ++m_param, ++n_param){
//                       if(m_param->baseType != (*n_param)->basetype)
//                       // Error: Parameters dont have the same types
//                           typeError(argument_type_mismatch);
//                   }
//               }
//               else{
//               // Error: Wrong number of parameters
//                   typeError(argument_number_mismatch);
//               }
//           }
//         }
//         else{
//         // Error: Can't find the Object Class in Table
//           typeError(undefined_class);
//         }
//     }
//     else{

//     }  
//   }
}

void TypeCheck::visitMemberAccessNode(MemberAccessNode* node) {
  // WRITEME: Replace with code if necessary
  node->visit_children(this);
  std::cout << "memberAccess\n";

  std::map<std::string, VariableInfo>::iterator v_iter;
  std::map<std::string, ClassInfo>::iterator c_iter;
  VariableTable* class_v_Table; 
  ClassInfo c_info;
  VariableInfo v_info;
  std::string v_className;
  std::string c_name;
  bool found = false;

  // Check local VariableTable
  v_iter = currentVariableTable->find(node->identifier_1->name);
  if(v_iter != currentVariableTable->end()){
  // Found in local VariableTable
    v_info = v_iter->second;
    v_className = v_info.type.objectClassName; //class the variable belongs too
    c_iter = classTable->find(v_className);
    if(c_iter != classTable->end()){
    // Class exist == check for id_2 in class
      c_info = c_iter->second;
      class_v_Table = c_info.members;
      v_iter = class_v_Table->find(node->identifier_2->name);
      if(v_iter == class_v_Table->end()){
      // Member does not exist in Class VariableTable == check super class
        v_className = c_info.superClassName;
        while(!found){
        // Extends to a super class
          c_iter = classTable->find(v_className);
          if(c_iter != classTable->end()){
            // Found superClass
            c_info = c_iter->second;
            class_v_Table = c_info.members;
            v_iter  = class_v_Table->find(node->identifier_2->name);
            if(v_className == ""){
              // Member is not found
              typeError(undefined_member);
            }
            if(v_iter == class_v_Table->end()){
              // Not found in superClass VariableTable
              v_className = c_info.superClassName;
            }
            else{
              found = true;
            }
          }
        }
      }
    }
    else{
    // Error: Class Object does not exist == variable doesn't not exist
      typeError(undefined_class); 
    }
  }
  else{
  // Check Class VariableTable for variable
    c_iter = classTable->find(currentClassName);
    if(c_iter != classTable->end()){
      // Class Found
      c_info = c_iter->second;
      class_v_Table = c_info.members;
      v_iter = class_v_Table->find(node->identifier_1->name);
      if(v_iter != class_v_Table->end()){
        // Found variable in Class VariableTable
        v_info = v_iter->second;
        v_className = v_info.type.objectClassName;
        c_iter = classTable->find(v_className);
        if(c_iter != classTable->end()){
        // Class exist == check for id_2 in class
          c_info = c_iter->second;
          class_v_Table = c_info.members;
          v_iter = class_v_Table->find(node->identifier_2->name);
          if(v_iter == class_v_Table->end()){
          // Member does not exist in Class VariableTable == check super class
            v_className = c_info.superClassName;
            while(!found){
            // Extends to a super class
              c_iter = classTable->find(v_className);
              if(c_iter != classTable->end()){
                // Found superClass
                c_info = c_iter->second;
                class_v_Table = c_info.members;
                v_iter  = class_v_Table->find(node->identifier_2->name);
                if(v_className == ""){
                  // Member is not found
                  typeError(undefined_member);
                }
                if(v_iter == class_v_Table->end()){
                  // Not found in superClass VariableTable
                  v_className = c_info.superClassName;
                }
                else{
                  found = true;
                }
              }
            }
          }
        }
        else{
        // Error: Class Object does not exist == variable doesn't not exist
          typeError(undefined_class); 
        }
      }
    }
    else{
      // Can't find current class in ClassTable = undefined_class
      typeError(undefined_class);
    }
  }
}

void TypeCheck::visitVariableNode(VariableNode* node) {
  // WRITEME: Replace with code if necessary
  std::map<std::string, VariableInfo>::iterator v_iter; 
  std::map<std::string, VariableInfo>::iterator super_v_iter; 
  std::map<std::string, ClassInfo>::iterator c_iter;
  std::map<std::string, ClassInfo>::iterator super_c_iter;
  VariableTable* v_table;
  VariableTable* super_v_table;
  VariableInfo v_info;
  ClassInfo c_info;
  ClassInfo super_c_info;
  std::string superName;
  bool found = true;

  v_iter = currentVariableTable->find(node->identifier->name);
  if(v_iter == currentVariableTable->end()){
    c_iter = classTable->find(currentClassName);
//    if(c_iter != classTable->end()){
    // Check Class VariableTable
      c_info = c_iter->second;
      v_table = c_info.members;
      v_iter = v_table->find(node->identifier->name);
      if(v_iter == v_table->end()){
      // Check superClass
        c_info = c_iter->second;
        superName = c_info.superClassName;
        while (!found){
          if(superName == ""){
          // Error: Method is not defined
              typeError(undefined_variable);
          }
          super_c_iter = classTable->find(superName);
          super_c_info = super_c_iter->second;
          super_v_table = super_c_info.members;
          super_v_iter = super_v_table->find(node->identifier->name);
          if(v_iter == super_v_table->end()){ // Move to super class
              superName = super_c_info.superClassName;
          }
          else{ // If var does exist
            found = true;
            v_info = v_iter->second;
            node->basetype = v_info.type.baseType;
            node->objectClassName = v_info.type.objectClassName;
          }
        } 
      }
      else{
        v_info = v_iter->second;
        node->basetype = v_info.type.baseType;
        node->objectClassName = v_info.type.objectClassName;
      }
    // }
    // else{
    // // Can't find currentClassTable in ClassTable
    //   //typeError(undefined_class); 
    // }
  }
  else{
    v_info = v_iter->second;
    node->basetype = v_info.type.baseType;
    node->objectClassName = v_info.type.objectClassName;
  }
}

void TypeCheck::visitIntegerLiteralNode(IntegerLiteralNode* node) {
  // WRITEME: Replace with code if necessary
  node->basetype = bt_integer;
}

void TypeCheck::visitBooleanLiteralNode(BooleanLiteralNode* node) {
  // WRITEME: Replace with code if necessary
  node->basetype = bt_boolean;
}

void TypeCheck::visitNewNode(NewNode* node) {
  // WRITEME: Replace with code if necessary
  node->visit_children(this);
}

void TypeCheck::visitIntegerTypeNode(IntegerTypeNode* node) {
  // WRITEME: Replace with code if necessary
  node->basetype = bt_integer;
}

void TypeCheck::visitBooleanTypeNode(BooleanTypeNode* node) {
  // WRITEME: Replace with code if necessary
  node->basetype = bt_boolean;
}

void TypeCheck::visitObjectTypeNode(ObjectTypeNode* node) {
  // WRITEME: Replace with code if necessary
  node->basetype = bt_object;
  node->objectClassName = node->identifier->name;
  std:: cout << node->objectClassName << "\n";
}

void TypeCheck::visitNoneNode(NoneNode* node) {
  // WRITEME: Replace with code if necessary
  node->basetype = bt_none;
}

void TypeCheck::visitIdentifierNode(IdentifierNode* node) {
  // WRITEME: Replace with code if necessary
}

void TypeCheck::visitIntegerNode(IntegerNode* node) {
  // WRITEME: Replace with code if necessary
  node->basetype = bt_integer;
}


// The following functions are used to print the Symbol Table.
// They do not need to be modified at all.

std::string genIndent(int indent) {
  std::string string = std::string("");
  for (int i = 0; i < indent; i++)
    string += std::string(" ");
  return string;
}

std::string string(CompoundType type) {
  switch (type.baseType) {
    case bt_integer:
      return std::string("Integer");
    case bt_boolean:
      return std::string("Boolean");
    case bt_none:
      return std::string("None");
    case bt_object:
      return std::string("Object(") + type.objectClassName + std::string(")");
    default:
      return std::string("");
  }
}


void print(VariableTable variableTable, int indent) {
  std::cout << genIndent(indent) << "VariableTable {";
  if (variableTable.size() == 0) {
    std::cout << "}";
    return;
  }
  std::cout << std::endl;
  for (VariableTable::iterator it = variableTable.begin(); it != variableTable.end(); it++) {
    std::cout << genIndent(indent + 2) << it->first << " -> {" << string(it->second.type);
    std::cout << ", " << it->second.offset << ", " << it->second.size << "}";
    if (it != --variableTable.end())
      std::cout << ",";
    std::cout << std::endl;
  }
  std::cout << genIndent(indent) << "}";
}

void print(MethodTable methodTable, int indent) {
  std::cout << genIndent(indent) << "MethodTable {";
  if (methodTable.size() == 0) {
    std::cout << "}";
    return;
  }
  std::cout << std::endl;
  for (MethodTable::iterator it = methodTable.begin(); it != methodTable.end(); it++) {
    std::cout << genIndent(indent + 2) << it->first << " -> {" << std::endl;
    std::cout << genIndent(indent + 4) << string(it->second.returnType) << "," << std::endl;
    std::cout << genIndent(indent + 4) << it->second.localsSize << "," << std::endl;
    print(*it->second.variables, indent + 4);
    std::cout <<std::endl;
    std::cout << genIndent(indent + 2) << "}";
    if (it != --methodTable.end())
      std::cout << ",";
    std::cout << std::endl;
  }
  std::cout << genIndent(indent) << "}";
}

void print(ClassTable classTable, int indent) {
  std::cout << genIndent(indent) << "ClassTable {" << std::endl;
  for (ClassTable::iterator it = classTable.begin(); it != classTable.end(); it++) {
    std::cout << genIndent(indent + 2) << it->first << " -> {" << std::endl;
    if (it->second.superClassName != "")
      std::cout << genIndent(indent + 4) << it->second.superClassName << "," << std::endl;
    print(*it->second.members, indent + 4);
    std::cout << "," << std::endl;
    print(*it->second.methods, indent + 4);
    std::cout <<std::endl;
    std::cout << genIndent(indent + 2) << "}";
    if (it != --classTable.end())
      std::cout << ",";
    std::cout << std::endl;
  }
  std::cout << genIndent(indent) << "}" << std::endl;
}

void print(ClassTable classTable) {
  print(classTable, 0);
}
