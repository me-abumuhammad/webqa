#ifndef DONASI_MODEL_HPP
#define DONASI_MODEL_HPP

#include <sqlpp11/table.h>
#include <sqlpp11/data_types.h>
#include <sqlpp11/char_sequence.h>

namespace model {
    namespace Donasi_ {
        struct IdDonasi {
            struct _alias_t {
                static constexpr const char _literal[] = "id_donasi";
                using _name_t = sqlpp::make_char_sequence<sizeof (_literal), _literal>;
                template<typename T>
                struct _member_t {
                    T id_donasi;
                    T& operator()() {
                        return id_donasi;
                    }
                    const T& operator()() const {
                        return id_donasi;
                    }
                };
            };
            using _traits = sqlpp::make_traits<sqlpp::bigint, sqlpp::tag::must_not_insert, sqlpp::tag::must_not_update>;
        };

        struct MuhsininId {
            struct _alias_t {
                static constexpr const char _literal[] = "muhsinin_id";
                using _name_t = sqlpp::make_char_sequence<sizeof (_literal), _literal>;
                template<typename T>
                struct _member_t {
                    T muhsinin_id;
                    T& operator()() {
                        return muhsinin_id;
                    }
                    const T& operator()() const {
                        return muhsinin_id;
                    }
                };
            };
            using _traits = sqlpp::make_traits<sqlpp::integer>;
        };

        struct Keterangan {
            struct _alias_t {
                static constexpr const char _literal[] = "keterangan";
                using _name_t = sqlpp::make_char_sequence<sizeof (_literal), _literal>;
                template<typename T>
                struct _member_t {
                    T keterangan;
                    T& operator()() {
                        return keterangan;
                    }
                    const T& operator()() const {
                        return keterangan;
                    }
                };
            };
            using _traits = sqlpp::make_traits<sqlpp::varchar>;
        };

        struct Nomor {
            struct _alias_t {
                static constexpr const char _literal[] = "nomor";
                using _name_t = sqlpp::make_char_sequence<sizeof (_literal), _literal>;
                template<typename T>
                struct _member_t {
                    T nomor;
                    T& operator()() {
                        return nomor;
                    }
                    const T& operator()() const {
                        return nomor;
                    }
                };
            };
            using _traits = sqlpp::make_traits<sqlpp::varchar>;
        };


    }
}

#endif // DONASI_MODEL_HPP
