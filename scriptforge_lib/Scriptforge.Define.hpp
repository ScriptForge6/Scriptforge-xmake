#ifndef SCRIPTFORGE_DEFINE_HPP
#define SCRIPTFORGE_DEFINE_HPP


#define _SF_BEGIN namespace Scriptforge {
#define _SF_END }
#define _SF /*::*/Scriptforge::


#define _SF_ARGV_BEGIN _SF_BEGIN inline namespace Argv {
#define _SF_ARGV_END   _SF_END }

#define _SF_ARGV _SF Argv::
#define _SF_ARGVCLI _SF_ARGV BasicArgvCli<HashT>::
#define _SF_ARGVCLI_TEM template<isHash HashT>


#define _SF_BASE_BEGIN _SF_BEGIN inline namespace Base {
#define _SF_BASE_END   _SF_END }

#define _SF_BASE _SF Base::
#define _SF_BASE64 _SF_BASE BasicBase64<CharSet>::
#define  _SF_BASE64_TEM template<isBaseCharSet CharSet>


#define _SF_BUFFER_BEGIN _SF_BEGIN inline namespace Buffer {
#define  _SF_BUFFER_END   _SF_END }

#define _SF_BUFFER_LITERALS_BEGIN inline namespace BufferLiterals {
#define _SF_BUFFER_LITERALS_END }

#define _SF_BUFFER _SF Buffer::
#define _SF_BYTEBUFFER_A _SF_BUFFER BasicByteBuffer<Alloc>
#define _SF_BYTEBUFFER _SF_BUFFER BasicByteBuffer<Alloc>::
#define _SF_BYTEBUFFER_TEM_BEGIN template <typename Alloc
#define _SF_BYTEBUFFER_TEM_END >
#define _SF_BYTEBUFFER_TEM _SF_BYTEBUFFER_TEM_BEGIN _SF_BYTEBUFFER_TEM_END


#define _SF_CODE_BEGIN _SF_BEGIN namespace Code {
#define _SF_CODE_END _SF_END } 
#define _SF_CODE _SF Code::


#define _SF_CONCEPT_BEGIN _SF_BEGIN namespace Concept {
#define _SF_CONCEPT_END _SF_END } 
#define _SF_CONCEPT _SF Concept::


#define _SF_ERR_BEGIN _SF_BEGIN inline namespace Err {
#define _SF_ERR_END   _SF_END }

#define _SF_ERR _SF Err::
#define _SF_ERROR_A _SF_ERR BasicError<CodeT, T, Clock>
#define _SF_ERROR _SF_ERR BasicError<CodeT, T, Clock>::
#define _SF_ERROR_TEM_A template <typename CodeT, typename T, typename Clock>
#define _SF_ERROR_TEM _SF_ERROR_TEM_A \
                          requires ErrorRequires<CodeT, T, Clock>


#define _SF_MSG_BEGIN _SF_BEGIN inline namespace Msg {
#define _SF_MSG_END   _SF_END }

#define _SF_MSG _SF Msg::
#define _SF_MESSAGE_A _SF_MSG BasicMessage<T, Clock>
#define _SF_MESSAGE _SF_MSG BasicMessage<T, Clock>::
#define _SF_MESSAGE_TEM_A template <typename T, typename Clock>
#define _SF_MESSAGE_TEM _SF_MESSAGE_TEM_A \
                          requires MessageRequires<T, Clock>


#define _SF_STRINGCONVERSION_BEGIN _SF_BEGIN inline namespace StringConversion {
#define _SF_STRINGCONVERSION_END _SF_END }
#define _SF_STRINGCONVERSION _SF StringConversion::


#define _SF_THREADERROR ThreadError<Async>::
#define _SF_THREADERROR_TEM template<bool Async>


#define _SF_THROW_BEGIN _SF_BEGIN inline namespace Throw {
#define _SF_THROW_END _SF_END }
#define _SF_THROW _SF Throw::

#endif