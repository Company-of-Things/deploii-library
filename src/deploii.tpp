/*************************************************************************************/

#include "deploii.h"
#include <type_traits>

/*************************************************************************************/

template <typename T, size_t length>
void Deploii::send(MsgPack::str_t dataStreamID, const T (&data)[length]) {
   // Workaround for sending char array as a string
   if constexpr (std::is_same<T, char>::value) {
      return send(dataStreamID, String(data));
   }

   MsgPack::Packer packer;
   struct msgStruct {
      MsgPack::str_t id;
      MsgPack::fix_arr_t<T, length> data;
      MSGPACK_DEFINE_MAP(id, data);
   };
   msgStruct msg;
   msg.id = dataStreamID;
   memcpy(&msg.data, data, sizeof(T) * length);

   packer.serialize(msg);
   _handler->send(packer.data(), packer.size());
}

/*************************************************************************************/

template <typename T>
void Deploii::send(MsgPack::str_t dataStreamID, T data) {
   // Workaround for sending chars in text format
   if constexpr (std::is_same<T, char>::value) {
      return send(dataStreamID, String(data));
   }

   MsgPack::Packer packer;
   struct msgStruct {
      MsgPack::str_t id;
      T data;
      MSGPACK_DEFINE_MAP(id, data);
   };
   msgStruct msg;
   msg.id = dataStreamID;
   msg.data = data;

   packer.serialize(msg);
   _handler->send(packer.data(), packer.size());
}

/*************************************************************************************/

/*
 * Forwards any arguments through the connect function into the corresponding 
 * overload at the handler.
 */
template <typename... Args>
void Deploii::connect(Args&&... args) {
   Serial.println("Deploii_connect");
   Serial.println(reinterpret_cast<uintptr_t>(_handler));
   _handler->connect(_boardID, std::forward<Args>(args)...);
}

/*************************************************************************************/
