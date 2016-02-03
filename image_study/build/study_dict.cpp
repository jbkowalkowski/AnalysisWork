// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME study_dict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "/home/jbk/docker/study/src/classes.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *studycLcLIntProduct_Dictionary();
   static void studycLcLIntProduct_TClassManip(TClass*);
   static void *new_studycLcLIntProduct(void *p = 0);
   static void *newArray_studycLcLIntProduct(Long_t size, void *p);
   static void delete_studycLcLIntProduct(void *p);
   static void deleteArray_studycLcLIntProduct(void *p);
   static void destruct_studycLcLIntProduct(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::study::IntProduct*)
   {
      ::study::IntProduct *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::study::IntProduct));
      static ::ROOT::TGenericClassInfo 
         instance("study::IntProduct", 10, "SimpleProducts.h", 26,
                  typeid(::study::IntProduct), DefineBehavior(ptr, ptr),
                  &studycLcLIntProduct_Dictionary, isa_proxy, 8,
                  sizeof(::study::IntProduct) );
      instance.SetNew(&new_studycLcLIntProduct);
      instance.SetNewArray(&newArray_studycLcLIntProduct);
      instance.SetDelete(&delete_studycLcLIntProduct);
      instance.SetDeleteArray(&deleteArray_studycLcLIntProduct);
      instance.SetDestructor(&destruct_studycLcLIntProduct);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::study::IntProduct*)
   {
      return GenerateInitInstanceLocal((::study::IntProduct*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::study::IntProduct*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *studycLcLIntProduct_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::study::IntProduct*)0x0)->GetClass();
      studycLcLIntProduct_TClassManip(theClass);
   return theClass;
   }

   static void studycLcLIntProduct_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *studycLcLDoubleProduct_Dictionary();
   static void studycLcLDoubleProduct_TClassManip(TClass*);
   static void *new_studycLcLDoubleProduct(void *p = 0);
   static void *newArray_studycLcLDoubleProduct(Long_t size, void *p);
   static void delete_studycLcLDoubleProduct(void *p);
   static void deleteArray_studycLcLDoubleProduct(void *p);
   static void destruct_studycLcLDoubleProduct(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::study::DoubleProduct*)
   {
      ::study::DoubleProduct *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::study::DoubleProduct));
      static ::ROOT::TGenericClassInfo 
         instance("study::DoubleProduct", 10, "SimpleProducts.h", 38,
                  typeid(::study::DoubleProduct), DefineBehavior(ptr, ptr),
                  &studycLcLDoubleProduct_Dictionary, isa_proxy, 8,
                  sizeof(::study::DoubleProduct) );
      instance.SetNew(&new_studycLcLDoubleProduct);
      instance.SetNewArray(&newArray_studycLcLDoubleProduct);
      instance.SetDelete(&delete_studycLcLDoubleProduct);
      instance.SetDeleteArray(&deleteArray_studycLcLDoubleProduct);
      instance.SetDestructor(&destruct_studycLcLDoubleProduct);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::study::DoubleProduct*)
   {
      return GenerateInitInstanceLocal((::study::DoubleProduct*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::study::DoubleProduct*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *studycLcLDoubleProduct_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::study::DoubleProduct*)0x0)->GetClass();
      studycLcLDoubleProduct_TClassManip(theClass);
   return theClass;
   }

   static void studycLcLDoubleProduct_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *studycLcLStringProduct_Dictionary();
   static void studycLcLStringProduct_TClassManip(TClass*);
   static void *new_studycLcLStringProduct(void *p = 0);
   static void *newArray_studycLcLStringProduct(Long_t size, void *p);
   static void delete_studycLcLStringProduct(void *p);
   static void deleteArray_studycLcLStringProduct(void *p);
   static void destruct_studycLcLStringProduct(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::study::StringProduct*)
   {
      ::study::StringProduct *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::study::StringProduct));
      static ::ROOT::TGenericClassInfo 
         instance("study::StringProduct", 10, "SimpleProducts.h", 47,
                  typeid(::study::StringProduct), DefineBehavior(ptr, ptr),
                  &studycLcLStringProduct_Dictionary, isa_proxy, 8,
                  sizeof(::study::StringProduct) );
      instance.SetNew(&new_studycLcLStringProduct);
      instance.SetNewArray(&newArray_studycLcLStringProduct);
      instance.SetDelete(&delete_studycLcLStringProduct);
      instance.SetDeleteArray(&deleteArray_studycLcLStringProduct);
      instance.SetDestructor(&destruct_studycLcLStringProduct);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::study::StringProduct*)
   {
      return GenerateInitInstanceLocal((::study::StringProduct*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::study::StringProduct*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *studycLcLStringProduct_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::study::StringProduct*)0x0)->GetClass();
      studycLcLStringProduct_TClassManip(theClass);
   return theClass;
   }

   static void studycLcLStringProduct_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *artcLcLWrapperlEstudycLcLIntProductgR_Dictionary();
   static void artcLcLWrapperlEstudycLcLIntProductgR_TClassManip(TClass*);
   static void *new_artcLcLWrapperlEstudycLcLIntProductgR(void *p = 0);
   static void *newArray_artcLcLWrapperlEstudycLcLIntProductgR(Long_t size, void *p);
   static void delete_artcLcLWrapperlEstudycLcLIntProductgR(void *p);
   static void deleteArray_artcLcLWrapperlEstudycLcLIntProductgR(void *p);
   static void destruct_artcLcLWrapperlEstudycLcLIntProductgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::art::Wrapper<study::IntProduct>*)
   {
      ::art::Wrapper<study::IntProduct> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::art::Wrapper<study::IntProduct>));
      static ::ROOT::TGenericClassInfo 
         instance("art::Wrapper<study::IntProduct>", ::art::Wrapper<study::IntProduct>::Class_Version(), "invalid", 18,
                  typeid(::art::Wrapper<study::IntProduct>), DefineBehavior(ptr, ptr),
                  &artcLcLWrapperlEstudycLcLIntProductgR_Dictionary, isa_proxy, 0,
                  sizeof(::art::Wrapper<study::IntProduct>) );
      instance.SetNew(&new_artcLcLWrapperlEstudycLcLIntProductgR);
      instance.SetNewArray(&newArray_artcLcLWrapperlEstudycLcLIntProductgR);
      instance.SetDelete(&delete_artcLcLWrapperlEstudycLcLIntProductgR);
      instance.SetDeleteArray(&deleteArray_artcLcLWrapperlEstudycLcLIntProductgR);
      instance.SetDestructor(&destruct_artcLcLWrapperlEstudycLcLIntProductgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::art::Wrapper<study::IntProduct>*)
   {
      return GenerateInitInstanceLocal((::art::Wrapper<study::IntProduct>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::art::Wrapper<study::IntProduct>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *artcLcLWrapperlEstudycLcLIntProductgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::art::Wrapper<study::IntProduct>*)0x0)->GetClass();
      artcLcLWrapperlEstudycLcLIntProductgR_TClassManip(theClass);
   return theClass;
   }

   static void artcLcLWrapperlEstudycLcLIntProductgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *artcLcLWrapperlEstudycLcLDoubleProductgR_Dictionary();
   static void artcLcLWrapperlEstudycLcLDoubleProductgR_TClassManip(TClass*);
   static void *new_artcLcLWrapperlEstudycLcLDoubleProductgR(void *p = 0);
   static void *newArray_artcLcLWrapperlEstudycLcLDoubleProductgR(Long_t size, void *p);
   static void delete_artcLcLWrapperlEstudycLcLDoubleProductgR(void *p);
   static void deleteArray_artcLcLWrapperlEstudycLcLDoubleProductgR(void *p);
   static void destruct_artcLcLWrapperlEstudycLcLDoubleProductgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::art::Wrapper<study::DoubleProduct>*)
   {
      ::art::Wrapper<study::DoubleProduct> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::art::Wrapper<study::DoubleProduct>));
      static ::ROOT::TGenericClassInfo 
         instance("art::Wrapper<study::DoubleProduct>", ::art::Wrapper<study::DoubleProduct>::Class_Version(), "art/Persistency/Common/Wrapper.h", 74,
                  typeid(::art::Wrapper<study::DoubleProduct>), DefineBehavior(ptr, ptr),
                  &artcLcLWrapperlEstudycLcLDoubleProductgR_Dictionary, isa_proxy, 0,
                  sizeof(::art::Wrapper<study::DoubleProduct>) );
      instance.SetNew(&new_artcLcLWrapperlEstudycLcLDoubleProductgR);
      instance.SetNewArray(&newArray_artcLcLWrapperlEstudycLcLDoubleProductgR);
      instance.SetDelete(&delete_artcLcLWrapperlEstudycLcLDoubleProductgR);
      instance.SetDeleteArray(&deleteArray_artcLcLWrapperlEstudycLcLDoubleProductgR);
      instance.SetDestructor(&destruct_artcLcLWrapperlEstudycLcLDoubleProductgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::art::Wrapper<study::DoubleProduct>*)
   {
      return GenerateInitInstanceLocal((::art::Wrapper<study::DoubleProduct>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::art::Wrapper<study::DoubleProduct>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *artcLcLWrapperlEstudycLcLDoubleProductgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::art::Wrapper<study::DoubleProduct>*)0x0)->GetClass();
      artcLcLWrapperlEstudycLcLDoubleProductgR_TClassManip(theClass);
   return theClass;
   }

   static void artcLcLWrapperlEstudycLcLDoubleProductgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *artcLcLWrapperlEstudycLcLStringProductgR_Dictionary();
   static void artcLcLWrapperlEstudycLcLStringProductgR_TClassManip(TClass*);
   static void *new_artcLcLWrapperlEstudycLcLStringProductgR(void *p = 0);
   static void *newArray_artcLcLWrapperlEstudycLcLStringProductgR(Long_t size, void *p);
   static void delete_artcLcLWrapperlEstudycLcLStringProductgR(void *p);
   static void deleteArray_artcLcLWrapperlEstudycLcLStringProductgR(void *p);
   static void destruct_artcLcLWrapperlEstudycLcLStringProductgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::art::Wrapper<study::StringProduct>*)
   {
      ::art::Wrapper<study::StringProduct> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::art::Wrapper<study::StringProduct>));
      static ::ROOT::TGenericClassInfo 
         instance("art::Wrapper<study::StringProduct>", ::art::Wrapper<study::StringProduct>::Class_Version(), "art/Persistency/Common/Wrapper.h", 74,
                  typeid(::art::Wrapper<study::StringProduct>), DefineBehavior(ptr, ptr),
                  &artcLcLWrapperlEstudycLcLStringProductgR_Dictionary, isa_proxy, 0,
                  sizeof(::art::Wrapper<study::StringProduct>) );
      instance.SetNew(&new_artcLcLWrapperlEstudycLcLStringProductgR);
      instance.SetNewArray(&newArray_artcLcLWrapperlEstudycLcLStringProductgR);
      instance.SetDelete(&delete_artcLcLWrapperlEstudycLcLStringProductgR);
      instance.SetDeleteArray(&deleteArray_artcLcLWrapperlEstudycLcLStringProductgR);
      instance.SetDestructor(&destruct_artcLcLWrapperlEstudycLcLStringProductgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::art::Wrapper<study::StringProduct>*)
   {
      return GenerateInitInstanceLocal((::art::Wrapper<study::StringProduct>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::art::Wrapper<study::StringProduct>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *artcLcLWrapperlEstudycLcLStringProductgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::art::Wrapper<study::StringProduct>*)0x0)->GetClass();
      artcLcLWrapperlEstudycLcLStringProductgR_TClassManip(theClass);
   return theClass;
   }

   static void artcLcLWrapperlEstudycLcLStringProductgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *artcLcLWrapperlEvectorlEstudycLcLIntProductgRsPgR_Dictionary();
   static void artcLcLWrapperlEvectorlEstudycLcLIntProductgRsPgR_TClassManip(TClass*);
   static void *new_artcLcLWrapperlEvectorlEstudycLcLIntProductgRsPgR(void *p = 0);
   static void *newArray_artcLcLWrapperlEvectorlEstudycLcLIntProductgRsPgR(Long_t size, void *p);
   static void delete_artcLcLWrapperlEvectorlEstudycLcLIntProductgRsPgR(void *p);
   static void deleteArray_artcLcLWrapperlEvectorlEstudycLcLIntProductgRsPgR(void *p);
   static void destruct_artcLcLWrapperlEvectorlEstudycLcLIntProductgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::art::Wrapper<vector<study::IntProduct> >*)
   {
      ::art::Wrapper<vector<study::IntProduct> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::art::Wrapper<vector<study::IntProduct> >));
      static ::ROOT::TGenericClassInfo 
         instance("art::Wrapper<vector<study::IntProduct> >", ::art::Wrapper<vector<study::IntProduct> >::Class_Version(), "invalid", 19,
                  typeid(::art::Wrapper<vector<study::IntProduct> >), DefineBehavior(ptr, ptr),
                  &artcLcLWrapperlEvectorlEstudycLcLIntProductgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(::art::Wrapper<vector<study::IntProduct> >) );
      instance.SetNew(&new_artcLcLWrapperlEvectorlEstudycLcLIntProductgRsPgR);
      instance.SetNewArray(&newArray_artcLcLWrapperlEvectorlEstudycLcLIntProductgRsPgR);
      instance.SetDelete(&delete_artcLcLWrapperlEvectorlEstudycLcLIntProductgRsPgR);
      instance.SetDeleteArray(&deleteArray_artcLcLWrapperlEvectorlEstudycLcLIntProductgRsPgR);
      instance.SetDestructor(&destruct_artcLcLWrapperlEvectorlEstudycLcLIntProductgRsPgR);

      ::ROOT::AddClassAlternate("art::Wrapper<vector<study::IntProduct> >","art::Wrapper<std::vector<study::IntProduct> >");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::art::Wrapper<vector<study::IntProduct> >*)
   {
      return GenerateInitInstanceLocal((::art::Wrapper<vector<study::IntProduct> >*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::art::Wrapper<vector<study::IntProduct> >*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *artcLcLWrapperlEvectorlEstudycLcLIntProductgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::art::Wrapper<vector<study::IntProduct> >*)0x0)->GetClass();
      artcLcLWrapperlEvectorlEstudycLcLIntProductgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void artcLcLWrapperlEvectorlEstudycLcLIntProductgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *artcLcLWrapperlEvectorlEstudycLcLDoubleProductgRsPgR_Dictionary();
   static void artcLcLWrapperlEvectorlEstudycLcLDoubleProductgRsPgR_TClassManip(TClass*);
   static void *new_artcLcLWrapperlEvectorlEstudycLcLDoubleProductgRsPgR(void *p = 0);
   static void *newArray_artcLcLWrapperlEvectorlEstudycLcLDoubleProductgRsPgR(Long_t size, void *p);
   static void delete_artcLcLWrapperlEvectorlEstudycLcLDoubleProductgRsPgR(void *p);
   static void deleteArray_artcLcLWrapperlEvectorlEstudycLcLDoubleProductgRsPgR(void *p);
   static void destruct_artcLcLWrapperlEvectorlEstudycLcLDoubleProductgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::art::Wrapper<vector<study::DoubleProduct> >*)
   {
      ::art::Wrapper<vector<study::DoubleProduct> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::art::Wrapper<vector<study::DoubleProduct> >));
      static ::ROOT::TGenericClassInfo 
         instance("art::Wrapper<vector<study::DoubleProduct> >", ::art::Wrapper<vector<study::DoubleProduct> >::Class_Version(), "invalid", 20,
                  typeid(::art::Wrapper<vector<study::DoubleProduct> >), DefineBehavior(ptr, ptr),
                  &artcLcLWrapperlEvectorlEstudycLcLDoubleProductgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(::art::Wrapper<vector<study::DoubleProduct> >) );
      instance.SetNew(&new_artcLcLWrapperlEvectorlEstudycLcLDoubleProductgRsPgR);
      instance.SetNewArray(&newArray_artcLcLWrapperlEvectorlEstudycLcLDoubleProductgRsPgR);
      instance.SetDelete(&delete_artcLcLWrapperlEvectorlEstudycLcLDoubleProductgRsPgR);
      instance.SetDeleteArray(&deleteArray_artcLcLWrapperlEvectorlEstudycLcLDoubleProductgRsPgR);
      instance.SetDestructor(&destruct_artcLcLWrapperlEvectorlEstudycLcLDoubleProductgRsPgR);

      ::ROOT::AddClassAlternate("art::Wrapper<vector<study::DoubleProduct> >","art::Wrapper<std::vector<study::DoubleProduct> >");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::art::Wrapper<vector<study::DoubleProduct> >*)
   {
      return GenerateInitInstanceLocal((::art::Wrapper<vector<study::DoubleProduct> >*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::art::Wrapper<vector<study::DoubleProduct> >*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *artcLcLWrapperlEvectorlEstudycLcLDoubleProductgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::art::Wrapper<vector<study::DoubleProduct> >*)0x0)->GetClass();
      artcLcLWrapperlEvectorlEstudycLcLDoubleProductgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void artcLcLWrapperlEvectorlEstudycLcLDoubleProductgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *artcLcLWrapperlEvectorlEstudycLcLStringProductgRsPgR_Dictionary();
   static void artcLcLWrapperlEvectorlEstudycLcLStringProductgRsPgR_TClassManip(TClass*);
   static void *new_artcLcLWrapperlEvectorlEstudycLcLStringProductgRsPgR(void *p = 0);
   static void *newArray_artcLcLWrapperlEvectorlEstudycLcLStringProductgRsPgR(Long_t size, void *p);
   static void delete_artcLcLWrapperlEvectorlEstudycLcLStringProductgRsPgR(void *p);
   static void deleteArray_artcLcLWrapperlEvectorlEstudycLcLStringProductgRsPgR(void *p);
   static void destruct_artcLcLWrapperlEvectorlEstudycLcLStringProductgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::art::Wrapper<vector<study::StringProduct> >*)
   {
      ::art::Wrapper<vector<study::StringProduct> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::art::Wrapper<vector<study::StringProduct> >));
      static ::ROOT::TGenericClassInfo 
         instance("art::Wrapper<vector<study::StringProduct> >", ::art::Wrapper<vector<study::StringProduct> >::Class_Version(), "invalid", 21,
                  typeid(::art::Wrapper<vector<study::StringProduct> >), DefineBehavior(ptr, ptr),
                  &artcLcLWrapperlEvectorlEstudycLcLStringProductgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(::art::Wrapper<vector<study::StringProduct> >) );
      instance.SetNew(&new_artcLcLWrapperlEvectorlEstudycLcLStringProductgRsPgR);
      instance.SetNewArray(&newArray_artcLcLWrapperlEvectorlEstudycLcLStringProductgRsPgR);
      instance.SetDelete(&delete_artcLcLWrapperlEvectorlEstudycLcLStringProductgRsPgR);
      instance.SetDeleteArray(&deleteArray_artcLcLWrapperlEvectorlEstudycLcLStringProductgRsPgR);
      instance.SetDestructor(&destruct_artcLcLWrapperlEvectorlEstudycLcLStringProductgRsPgR);

      ::ROOT::AddClassAlternate("art::Wrapper<vector<study::StringProduct> >","art::Wrapper<std::vector<study::StringProduct> >");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::art::Wrapper<vector<study::StringProduct> >*)
   {
      return GenerateInitInstanceLocal((::art::Wrapper<vector<study::StringProduct> >*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::art::Wrapper<vector<study::StringProduct> >*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *artcLcLWrapperlEvectorlEstudycLcLStringProductgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::art::Wrapper<vector<study::StringProduct> >*)0x0)->GetClass();
      artcLcLWrapperlEvectorlEstudycLcLStringProductgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void artcLcLWrapperlEvectorlEstudycLcLStringProductgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_studycLcLIntProduct(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) ::study::IntProduct : new ::study::IntProduct;
   }
   static void *newArray_studycLcLIntProduct(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) ::study::IntProduct[nElements] : new ::study::IntProduct[nElements];
   }
   // Wrapper around operator delete
   static void delete_studycLcLIntProduct(void *p) {
      delete ((::study::IntProduct*)p);
   }
   static void deleteArray_studycLcLIntProduct(void *p) {
      delete [] ((::study::IntProduct*)p);
   }
   static void destruct_studycLcLIntProduct(void *p) {
      typedef ::study::IntProduct current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::study::IntProduct

namespace ROOT {
   // Wrappers around operator new
   static void *new_studycLcLDoubleProduct(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) ::study::DoubleProduct : new ::study::DoubleProduct;
   }
   static void *newArray_studycLcLDoubleProduct(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) ::study::DoubleProduct[nElements] : new ::study::DoubleProduct[nElements];
   }
   // Wrapper around operator delete
   static void delete_studycLcLDoubleProduct(void *p) {
      delete ((::study::DoubleProduct*)p);
   }
   static void deleteArray_studycLcLDoubleProduct(void *p) {
      delete [] ((::study::DoubleProduct*)p);
   }
   static void destruct_studycLcLDoubleProduct(void *p) {
      typedef ::study::DoubleProduct current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::study::DoubleProduct

namespace ROOT {
   // Wrappers around operator new
   static void *new_studycLcLStringProduct(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) ::study::StringProduct : new ::study::StringProduct;
   }
   static void *newArray_studycLcLStringProduct(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) ::study::StringProduct[nElements] : new ::study::StringProduct[nElements];
   }
   // Wrapper around operator delete
   static void delete_studycLcLStringProduct(void *p) {
      delete ((::study::StringProduct*)p);
   }
   static void deleteArray_studycLcLStringProduct(void *p) {
      delete [] ((::study::StringProduct*)p);
   }
   static void destruct_studycLcLStringProduct(void *p) {
      typedef ::study::StringProduct current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::study::StringProduct

namespace ROOT {
   // Wrappers around operator new
   static void *new_artcLcLWrapperlEstudycLcLIntProductgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) ::art::Wrapper<study::IntProduct> : new ::art::Wrapper<study::IntProduct>;
   }
   static void *newArray_artcLcLWrapperlEstudycLcLIntProductgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) ::art::Wrapper<study::IntProduct>[nElements] : new ::art::Wrapper<study::IntProduct>[nElements];
   }
   // Wrapper around operator delete
   static void delete_artcLcLWrapperlEstudycLcLIntProductgR(void *p) {
      delete ((::art::Wrapper<study::IntProduct>*)p);
   }
   static void deleteArray_artcLcLWrapperlEstudycLcLIntProductgR(void *p) {
      delete [] ((::art::Wrapper<study::IntProduct>*)p);
   }
   static void destruct_artcLcLWrapperlEstudycLcLIntProductgR(void *p) {
      typedef ::art::Wrapper<study::IntProduct> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::art::Wrapper<study::IntProduct>

namespace ROOT {
   // Wrappers around operator new
   static void *new_artcLcLWrapperlEstudycLcLDoubleProductgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) ::art::Wrapper<study::DoubleProduct> : new ::art::Wrapper<study::DoubleProduct>;
   }
   static void *newArray_artcLcLWrapperlEstudycLcLDoubleProductgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) ::art::Wrapper<study::DoubleProduct>[nElements] : new ::art::Wrapper<study::DoubleProduct>[nElements];
   }
   // Wrapper around operator delete
   static void delete_artcLcLWrapperlEstudycLcLDoubleProductgR(void *p) {
      delete ((::art::Wrapper<study::DoubleProduct>*)p);
   }
   static void deleteArray_artcLcLWrapperlEstudycLcLDoubleProductgR(void *p) {
      delete [] ((::art::Wrapper<study::DoubleProduct>*)p);
   }
   static void destruct_artcLcLWrapperlEstudycLcLDoubleProductgR(void *p) {
      typedef ::art::Wrapper<study::DoubleProduct> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::art::Wrapper<study::DoubleProduct>

namespace ROOT {
   // Wrappers around operator new
   static void *new_artcLcLWrapperlEstudycLcLStringProductgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) ::art::Wrapper<study::StringProduct> : new ::art::Wrapper<study::StringProduct>;
   }
   static void *newArray_artcLcLWrapperlEstudycLcLStringProductgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) ::art::Wrapper<study::StringProduct>[nElements] : new ::art::Wrapper<study::StringProduct>[nElements];
   }
   // Wrapper around operator delete
   static void delete_artcLcLWrapperlEstudycLcLStringProductgR(void *p) {
      delete ((::art::Wrapper<study::StringProduct>*)p);
   }
   static void deleteArray_artcLcLWrapperlEstudycLcLStringProductgR(void *p) {
      delete [] ((::art::Wrapper<study::StringProduct>*)p);
   }
   static void destruct_artcLcLWrapperlEstudycLcLStringProductgR(void *p) {
      typedef ::art::Wrapper<study::StringProduct> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::art::Wrapper<study::StringProduct>

namespace ROOT {
   // Wrappers around operator new
   static void *new_artcLcLWrapperlEvectorlEstudycLcLIntProductgRsPgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) ::art::Wrapper<vector<study::IntProduct> > : new ::art::Wrapper<vector<study::IntProduct> >;
   }
   static void *newArray_artcLcLWrapperlEvectorlEstudycLcLIntProductgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) ::art::Wrapper<vector<study::IntProduct> >[nElements] : new ::art::Wrapper<vector<study::IntProduct> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_artcLcLWrapperlEvectorlEstudycLcLIntProductgRsPgR(void *p) {
      delete ((::art::Wrapper<vector<study::IntProduct> >*)p);
   }
   static void deleteArray_artcLcLWrapperlEvectorlEstudycLcLIntProductgRsPgR(void *p) {
      delete [] ((::art::Wrapper<vector<study::IntProduct> >*)p);
   }
   static void destruct_artcLcLWrapperlEvectorlEstudycLcLIntProductgRsPgR(void *p) {
      typedef ::art::Wrapper<vector<study::IntProduct> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::art::Wrapper<vector<study::IntProduct> >

namespace ROOT {
   // Wrappers around operator new
   static void *new_artcLcLWrapperlEvectorlEstudycLcLDoubleProductgRsPgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) ::art::Wrapper<vector<study::DoubleProduct> > : new ::art::Wrapper<vector<study::DoubleProduct> >;
   }
   static void *newArray_artcLcLWrapperlEvectorlEstudycLcLDoubleProductgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) ::art::Wrapper<vector<study::DoubleProduct> >[nElements] : new ::art::Wrapper<vector<study::DoubleProduct> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_artcLcLWrapperlEvectorlEstudycLcLDoubleProductgRsPgR(void *p) {
      delete ((::art::Wrapper<vector<study::DoubleProduct> >*)p);
   }
   static void deleteArray_artcLcLWrapperlEvectorlEstudycLcLDoubleProductgRsPgR(void *p) {
      delete [] ((::art::Wrapper<vector<study::DoubleProduct> >*)p);
   }
   static void destruct_artcLcLWrapperlEvectorlEstudycLcLDoubleProductgRsPgR(void *p) {
      typedef ::art::Wrapper<vector<study::DoubleProduct> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::art::Wrapper<vector<study::DoubleProduct> >

namespace ROOT {
   // Wrappers around operator new
   static void *new_artcLcLWrapperlEvectorlEstudycLcLStringProductgRsPgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) ::art::Wrapper<vector<study::StringProduct> > : new ::art::Wrapper<vector<study::StringProduct> >;
   }
   static void *newArray_artcLcLWrapperlEvectorlEstudycLcLStringProductgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) ::art::Wrapper<vector<study::StringProduct> >[nElements] : new ::art::Wrapper<vector<study::StringProduct> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_artcLcLWrapperlEvectorlEstudycLcLStringProductgRsPgR(void *p) {
      delete ((::art::Wrapper<vector<study::StringProduct> >*)p);
   }
   static void deleteArray_artcLcLWrapperlEvectorlEstudycLcLStringProductgRsPgR(void *p) {
      delete [] ((::art::Wrapper<vector<study::StringProduct> >*)p);
   }
   static void destruct_artcLcLWrapperlEvectorlEstudycLcLStringProductgRsPgR(void *p) {
      typedef ::art::Wrapper<vector<study::StringProduct> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::art::Wrapper<vector<study::StringProduct> >

namespace ROOT {
   static TClass *vectorlEstudycLcLStringProductgR_Dictionary();
   static void vectorlEstudycLcLStringProductgR_TClassManip(TClass*);
   static void *new_vectorlEstudycLcLStringProductgR(void *p = 0);
   static void *newArray_vectorlEstudycLcLStringProductgR(Long_t size, void *p);
   static void delete_vectorlEstudycLcLStringProductgR(void *p);
   static void deleteArray_vectorlEstudycLcLStringProductgR(void *p);
   static void destruct_vectorlEstudycLcLStringProductgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<study::StringProduct>*)
   {
      vector<study::StringProduct> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<study::StringProduct>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<study::StringProduct>", -2, "vector", 214,
                  typeid(vector<study::StringProduct>), DefineBehavior(ptr, ptr),
                  &vectorlEstudycLcLStringProductgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<study::StringProduct>) );
      instance.SetNew(&new_vectorlEstudycLcLStringProductgR);
      instance.SetNewArray(&newArray_vectorlEstudycLcLStringProductgR);
      instance.SetDelete(&delete_vectorlEstudycLcLStringProductgR);
      instance.SetDeleteArray(&deleteArray_vectorlEstudycLcLStringProductgR);
      instance.SetDestructor(&destruct_vectorlEstudycLcLStringProductgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<study::StringProduct> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<study::StringProduct>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEstudycLcLStringProductgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<study::StringProduct>*)0x0)->GetClass();
      vectorlEstudycLcLStringProductgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEstudycLcLStringProductgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEstudycLcLStringProductgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<study::StringProduct> : new vector<study::StringProduct>;
   }
   static void *newArray_vectorlEstudycLcLStringProductgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<study::StringProduct>[nElements] : new vector<study::StringProduct>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEstudycLcLStringProductgR(void *p) {
      delete ((vector<study::StringProduct>*)p);
   }
   static void deleteArray_vectorlEstudycLcLStringProductgR(void *p) {
      delete [] ((vector<study::StringProduct>*)p);
   }
   static void destruct_vectorlEstudycLcLStringProductgR(void *p) {
      typedef vector<study::StringProduct> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<study::StringProduct>

namespace ROOT {
   static TClass *vectorlEstudycLcLIntProductgR_Dictionary();
   static void vectorlEstudycLcLIntProductgR_TClassManip(TClass*);
   static void *new_vectorlEstudycLcLIntProductgR(void *p = 0);
   static void *newArray_vectorlEstudycLcLIntProductgR(Long_t size, void *p);
   static void delete_vectorlEstudycLcLIntProductgR(void *p);
   static void deleteArray_vectorlEstudycLcLIntProductgR(void *p);
   static void destruct_vectorlEstudycLcLIntProductgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<study::IntProduct>*)
   {
      vector<study::IntProduct> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<study::IntProduct>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<study::IntProduct>", -2, "vector", 214,
                  typeid(vector<study::IntProduct>), DefineBehavior(ptr, ptr),
                  &vectorlEstudycLcLIntProductgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<study::IntProduct>) );
      instance.SetNew(&new_vectorlEstudycLcLIntProductgR);
      instance.SetNewArray(&newArray_vectorlEstudycLcLIntProductgR);
      instance.SetDelete(&delete_vectorlEstudycLcLIntProductgR);
      instance.SetDeleteArray(&deleteArray_vectorlEstudycLcLIntProductgR);
      instance.SetDestructor(&destruct_vectorlEstudycLcLIntProductgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<study::IntProduct> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<study::IntProduct>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEstudycLcLIntProductgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<study::IntProduct>*)0x0)->GetClass();
      vectorlEstudycLcLIntProductgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEstudycLcLIntProductgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEstudycLcLIntProductgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<study::IntProduct> : new vector<study::IntProduct>;
   }
   static void *newArray_vectorlEstudycLcLIntProductgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<study::IntProduct>[nElements] : new vector<study::IntProduct>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEstudycLcLIntProductgR(void *p) {
      delete ((vector<study::IntProduct>*)p);
   }
   static void deleteArray_vectorlEstudycLcLIntProductgR(void *p) {
      delete [] ((vector<study::IntProduct>*)p);
   }
   static void destruct_vectorlEstudycLcLIntProductgR(void *p) {
      typedef vector<study::IntProduct> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<study::IntProduct>

namespace ROOT {
   static TClass *vectorlEstudycLcLDoubleProductgR_Dictionary();
   static void vectorlEstudycLcLDoubleProductgR_TClassManip(TClass*);
   static void *new_vectorlEstudycLcLDoubleProductgR(void *p = 0);
   static void *newArray_vectorlEstudycLcLDoubleProductgR(Long_t size, void *p);
   static void delete_vectorlEstudycLcLDoubleProductgR(void *p);
   static void deleteArray_vectorlEstudycLcLDoubleProductgR(void *p);
   static void destruct_vectorlEstudycLcLDoubleProductgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<study::DoubleProduct>*)
   {
      vector<study::DoubleProduct> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<study::DoubleProduct>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<study::DoubleProduct>", -2, "vector", 214,
                  typeid(vector<study::DoubleProduct>), DefineBehavior(ptr, ptr),
                  &vectorlEstudycLcLDoubleProductgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<study::DoubleProduct>) );
      instance.SetNew(&new_vectorlEstudycLcLDoubleProductgR);
      instance.SetNewArray(&newArray_vectorlEstudycLcLDoubleProductgR);
      instance.SetDelete(&delete_vectorlEstudycLcLDoubleProductgR);
      instance.SetDeleteArray(&deleteArray_vectorlEstudycLcLDoubleProductgR);
      instance.SetDestructor(&destruct_vectorlEstudycLcLDoubleProductgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<study::DoubleProduct> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<study::DoubleProduct>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEstudycLcLDoubleProductgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<study::DoubleProduct>*)0x0)->GetClass();
      vectorlEstudycLcLDoubleProductgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEstudycLcLDoubleProductgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEstudycLcLDoubleProductgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<study::DoubleProduct> : new vector<study::DoubleProduct>;
   }
   static void *newArray_vectorlEstudycLcLDoubleProductgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<study::DoubleProduct>[nElements] : new vector<study::DoubleProduct>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEstudycLcLDoubleProductgR(void *p) {
      delete ((vector<study::DoubleProduct>*)p);
   }
   static void deleteArray_vectorlEstudycLcLDoubleProductgR(void *p) {
      delete [] ((vector<study::DoubleProduct>*)p);
   }
   static void destruct_vectorlEstudycLcLDoubleProductgR(void *p) {
      typedef vector<study::DoubleProduct> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<study::DoubleProduct>

namespace {
  void TriggerDictionaryInitialization_libstudy_dict_Impl() {
    static const char* headers[] = {
0    };
    static const char* includePaths[] = {
"/home/jbk/docker/study/src",
"/home/jbk/docker/study/src",
"/home/jbk/docker/study/build",
"/home/jbk/docker/study/src",
"/products/cppunit/v1_12_1c/Linux64bit+2.6-2.12-e9-prof/include",
"/products/clhep/v2_2_0_8/Linux64bit+2.6-2.12-e9-prof/include",
"/products/boost/v1_57_0a/Linux64bit+2.6-2.12-e9-prof/include",
"/products/cetlib/v1_17_01/include",
"/products/cetlib/v1_17_01/include",
"/products/sqlite/v3_08_10_02/Linux64bit+2.6-2.12/include",
"/products/fhiclcpp/v3_18_00/include",
"/products/fhiclcpp/v3_18_00/include",
"/products/sqlite/v3_08_10_02/Linux64bit+2.6-2.12/include",
"/products/sqlite/v3_08_10_02/Linux64bit+2.6-2.12/include",
"/products/messagefacility/v1_16_18/include",
"/products/messagefacility/v1_16_18/include",
"/products/root/v6_04_06/Linux64bit+2.6-2.12-e9-prof/include",
"/products/root/v6_04_06/Linux64bit+2.6-2.12-e9-prof/include",
"/products/tbb/v4_4_0/Linux64bit+2.6-2.12-e9-prof/include",
"/products/art/v1_18_03/include",
"/products/art/v1_18_03/include",
"/products/root/v6_04_06/Linux64bit+2.6-2.12-e9-prof/include",
"/home/jbk/docker/study/build/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace study{struct __attribute__((annotate("$clingAutoload$SimpleProducts.h")))  IntProduct;}
namespace study{struct __attribute__((annotate("$clingAutoload$SimpleProducts.h")))  DoubleProduct;}
namespace study{struct __attribute__((annotate("$clingAutoload$SimpleProducts.h")))  StringProduct;}
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$string")))  allocator;
}
namespace art{template <typename T> class __attribute__((annotate("$clingAutoload$art/Persistency/Common/Assns.h")))  Wrapper;
}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif
#ifndef NDEBUG
  #define NDEBUG 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "art/Persistency/Common/Assns.h"
#include "art/Persistency/Common/Ptr.h"
#include "art/Persistency/Common/PtrVector.h"
#include "art/Persistency/Common/Wrapper.h"

#include "SimpleProducts.h"

#include <vector>

template class art::Wrapper<study::IntProduct>;
template class art::Wrapper<std::vector<study::IntProduct> >;
template class art::Wrapper<std::vector<study::DoubleProduct> >;
template class art::Wrapper<std::vector<study::StringProduct> >;

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"art::Wrapper<std::vector<study::DoubleProduct> >", payloadCode, "@",
"art::Wrapper<std::vector<study::IntProduct> >", payloadCode, "@",
"art::Wrapper<std::vector<study::StringProduct> >", payloadCode, "@",
"art::Wrapper<study::DoubleProduct>", payloadCode, "@",
"art::Wrapper<study::IntProduct>", payloadCode, "@",
"art::Wrapper<study::StringProduct>", payloadCode, "@",
"art::Wrapper<vector<study::DoubleProduct> >", payloadCode, "@",
"art::Wrapper<vector<study::IntProduct> >", payloadCode, "@",
"art::Wrapper<vector<study::StringProduct> >", payloadCode, "@",
"study::DoubleProduct", payloadCode, "@",
"study::IntProduct", payloadCode, "@",
"study::StringProduct", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libstudy_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libstudy_dict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libstudy_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libstudy_dict() {
  TriggerDictionaryInitialization_libstudy_dict_Impl();
}
