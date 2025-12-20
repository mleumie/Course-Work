    #pragma once
    #ifndef TRANSACTIONS_H
    #define TRANSACTIONS_H

    #include <iostream>
    #include <memory>

    template <class T>
    class Transaction {
    private:
        std::unique_ptr<T> prev_state, curr_state;
    public:
        Transaction() {
            curr_state = std::make_unique<T>();
        }

        Transaction(T* obj) {
            curr_state = std::make_unique<T>();
            *curr_state = *obj;
        }

        Transaction(const Transaction& operation) {
            curr_state.reset(operation.curr_state.release());
        }

        ~Transaction() {}

        Transaction& operator=(const Transaction& operation) {
            if (this != operation) {
                curr_state.reset();
                curr_state = std::move(operation.curr_state);
            }

            return *this;
        }

        std::unique_ptr<T> operator->() {
            return curr_state;
        }

        bool startTransaction(T* temp) {
            prev_state.reset();

            prev_state = std::move(curr_state);
            curr_state = std::move(prev_state);

            if (!curr_state) {
                return false;
            }

            *curr_state = *temp;

            return true;
        }

        T commitTransaction() {
            T* temp = new T(*curr_state.get());
            prev_state.reset();

            return *temp;
        }

        bool denyTransaction() {
            if (prev_state) {
                curr_state = std::move(prev_state);
                prev_state.reset();

                return 1;
            }

            return 0;
        }
    };

    #endif