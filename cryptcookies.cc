#include <nan.h>
#include <Dpapi.h>
#include <iostream>

#pragma comment(lib, "crypt32.lib")

bool  EncryptString(const std::string& plaintext,
    std::string* ciphertext) {
    DATA_BLOB input;
    input.pbData = const_cast<BYTE*>(
        reinterpret_cast<const BYTE*>(plaintext.data()));
    input.cbData = static_cast<DWORD>(plaintext.length());
   // std::cout << plaintext << "\n";
    DATA_BLOB output;
    BOOL result =
        CryptProtectData(&input, L"", nullptr, nullptr, nullptr, 0, &output);
    if (!result) {
        Nan::ThrowTypeError("Failed to encrypt");
        return false;
    }

    // this does a copy
    ciphertext->assign(reinterpret_cast<std::string::value_type*>(output.pbData),
        output.cbData);

    LocalFree(output.pbData);
    return true;
}

static bool DecryptString(const std::string& ciphertext,
    std::string* plaintext) {
    DATA_BLOB input;
    input.pbData = const_cast<BYTE*>(
        reinterpret_cast<const BYTE*>(ciphertext.data()));
    input.cbData = static_cast<DWORD>(ciphertext.length());
    //std::cout << ciphertext;
    DATA_BLOB output;
    BOOL result = CryptUnprotectData(&input, nullptr, nullptr, nullptr, nullptr,
        0, &output);
    if (!result) {
        Nan::ThrowTypeError("Failed to decrypt");
        return false;
    }

    plaintext->assign(reinterpret_cast<char*>(output.pbData), output.cbData);
    LocalFree(output.pbData);
    return true;
}
void test(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  
  info.GetReturnValue().Set(Nan::New("this is a test addon!").ToLocalChecked());
}


void decodeCookieValue(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();

    if (info.Length() < 2) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    if (!info[0]->IsString() || !info[1]->IsUint8Array()) {
        Nan::ThrowTypeError("Wrong arguments");
        return;
    }

    int len = info[1].As<v8::Uint8Array>()->ByteLength();
    BYTE* row = (BYTE*)malloc(len);
    info[1].As<v8::Uint8Array>()->CopyContents(row, len);
    //for(int  i = 0 ; i < len;i++) printf("%d ",row[i]);
    
    std::string out, in(reinterpret_cast<char*>(row), len);
    free(row);
    if (DecryptString(in, &out)) {
        //printf("length:%d\n",out.length());
        v8::MaybeLocal<v8::String> retStr = v8::String::NewFromOneByte(info.GetIsolate(), (uint8_t*)out.c_str(), v8::NewStringType::kNormal, out.length());

        info.GetReturnValue().Set(retStr.ToLocalChecked());
    }
    else info.GetReturnValue().Set(Nan::Null());
}


void encodeCookieValue(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
 
    if (info.Length() < 2) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }
    if (!info[0]->IsString() || !info[1]->IsUint8Array()) {
        Nan::ThrowTypeError("Wrong arguments");
        return;
    }
  
    v8::Local<v8::String> _in = info[1].As<v8::String>();
    int len = info[1].As<v8::Uint8Array>()->ByteLength();
    BYTE* row = (BYTE*)malloc(len);
    info[1].As<v8::Uint8Array>()->CopyContents(row, len);
    std::string out,in(reinterpret_cast<char*>(row), len);
    //std::cout << );
    if (EncryptString(in, &out))//std::string((char*)"test=1")
    {
        //printf("length:%d\n", out.length());
        v8::MaybeLocal<v8::String> retStr = v8::String::NewFromOneByte(info.GetIsolate(),(uint8_t *) out.c_str(), v8::NewStringType::kNormal, out.length());
        info.GetReturnValue().Set(Nan::CopyBuffer((char*)out.data(), out.length()).ToLocalChecked());
    }
    else info.GetReturnValue().Set(Nan::Null());
}

void Init(v8::Local<v8::Object> exports) {
  v8::Local<v8::Context> context = exports->CreationContext();
  exports->Set(context,
               Nan::New("test").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(test)
                   ->GetFunction(context)
                   .ToLocalChecked());
  exports->Set(context,
              Nan::New("decodeCookieValue").ToLocalChecked(),
              Nan::New<v8::FunctionTemplate>(decodeCookieValue)
              ->GetFunction(context)
              .ToLocalChecked());
  exports->Set(context,
              Nan::New("encodeCookieValue").ToLocalChecked(),
              Nan::New<v8::FunctionTemplate>(encodeCookieValue)
              ->GetFunction(context)
              .ToLocalChecked());

  
}

NODE_MODULE(cookie_handle, Init)
