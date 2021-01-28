#ifndef SERIALIZATION_H
#define SERIALIZATION_H


#include <string>
#include <sstream>
#include <QByteArray>
#include <QString>
#include <map>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>


#define SERIALIZE(...)         \
  template <typename Archive>  \
  void serialize(Archive& a) { \
    a(__VA_ARGS__);            \
  };


/// map serialization


namespace cereal
{
    //! Saving for std::map<std::string, std::string> for text based archives
    // Note that this shows off some internal cereal traits such as EnableIf,
    // which will only allow this template to be instantiated if its predicates
    // are true
    template <class Archive, class C, class A,
            traits::EnableIf<traits::is_text_archive<Archive>::value> = traits::sfinae> inline
    void save( Archive & ar, std::map<QString, QString, C, A> const & map ) {
        for( const auto & i : map ) {
            ar(cereal::make_nvp(i.first.toStdString(), i.second.toStdString()));
        }
    }

    //! Loading for std::map<std::string, std::string> for text based archives
    template <class Archive, class C, class A,
            traits::EnableIf<traits::is_text_archive<Archive>::value> = traits::sfinae> inline
    void load( Archive & ar, std::map<QString, QString, C, A> & map ) {
        map.clear();

        auto hint = map.begin();
        while (true) {
            const auto namePtr = ar.getNodeName();
            if( !namePtr )
                break;

            std::string key = namePtr;
            std::string value;
            ar(value);

            hint = map.emplace_hint( hint, QString::fromUtf8(key.c_str()), QString::fromUtf8(value.c_str()));
        }
    }
} // namespace cereal


///



struct Archives {
    using InputArchive = cereal::JSONInputArchive;
    using OutputArchive = cereal::JSONOutputArchive;
    using BinaryInputArchive = cereal::BinaryInputArchive;
    using BinaryOutputArchive = cereal::BinaryOutputArchive;
};


template <typename T>
QByteArray Serialize(const T& object) {
    std::stringstream output;
    {
        Archives::OutputArchive oarchive(output);
        oarchive(object);
    }  // archive goes out of scope, ensuring all contents are flushed

    auto str = output.str();
    return QByteArray(str.c_str(), str.size());
}


template <typename T>
T Deserialize(const QByteArray& bytes) {
    std::string str = bytes.toStdString();

    T object;

    std::stringstream input(str);
    {
        Archives::InputArchive iarchive(input);
        iarchive(object);
    }  // archive goes out of scope, ensuring all contents are flushed

    return object;
}



template <typename T>
QByteArray SerializeBinary(const T& object) {
    std::stringstream output;
    {
        Archives::BinaryOutputArchive oarchive(output);
        oarchive(object);
    }  // archive goes out of scope, ensuring all contents are flushed

    auto str = output.str();
    return QByteArray(str.c_str(), str.size());
}


template <typename T>
T DeserializeBinary(const QByteArray& bytes) {
    std::string str = bytes.toStdString();

    T object;

    std::stringstream input(str);
    {
        Archives::BinaryInputArchive iarchive(input);
        iarchive(object);
    }  // archive goes out of scope, ensuring all contents are flushed

    return object;
}


#endif // SERIALIZATION_H
