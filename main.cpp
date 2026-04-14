#include <iostream>
#include <string>
#include <stdexcept>

#include "Sequence.hpp"
#include "SequenceAlgorithms.hpp"
#include "MutableArraySequence.hpp"
#include "ImmutableArraySequence.hpp"
#include "ListSequence.hpp"
#include "Bit.hpp"
#include "BitSequence.hpp"

int square_int(const int& x)
{
    return x * x;
}

bool is_even_int(const int& x)
{
    return x % 2 == 0;
}

int sum_reduce_int(const int& acc, const int& value)
{
    return acc + value;
}

Bit invert_bit(const Bit& b)
{
    return ~b;
}

bool is_true_bit(const Bit& b)
{
    return b.GetValue();
}

Bit xor_reduce_bit(const Bit& acc, const Bit& value)
{
    return acc ^ value;
}

void ClearInput()
{
    std::cin.clear();
    while (std::cin.get() != '\n' && std::cin)
    {
    }
}

int ReadInt(const std::string& prompt)
{
    int value;
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value)
        {
            return value;
        }

        std::cout << "Invalid integer. Try again.\n";
        ClearInput();
    }
}

int* ReadIntArray(int& count)
{
    count = ReadInt("Length: ");
    if (count < 0)
    {
        throw std::invalid_argument("Length cannot be negative");
    }

    if (count == 0)
    {
        return nullptr;
    }

    int* data = new int[count];
    for (int i = 0; i < count; ++i)
    {
        data[i] = ReadInt("value[" + std::to_string(i) + "]: ");
    }

    return data;
}

Bit* ReadBitArrayFromString(int& count)
{
    std::string s;
    std::cout << "Enter bit string (example: 10101): ";
    std::cin >> s;

    count = static_cast<int>(s.size());

    if (count == 0)
    {
        return nullptr;
    }

    Bit* data = new Bit[count];

    for (int i = 0; i < count; ++i)
    {
        if (s[i] == '0')
        {
            data[i] = Bit(false);
        }
        else if (s[i] == '1')
        {
            data[i] = Bit(true);
        }
        else
        {
            delete[] data;
            throw std::invalid_argument("Bit string must contain only 0 and 1");
        }
    }

    return data;
}

template<typename T>
void ReplaceSequence(Sequence<T>*& target, Sequence<T>* result)
{
    if (result != target)
    {
        delete target;
        target = result;
    }
}

void ReplaceBitSequence(BitSequence*& target, Sequence<Bit>* result)
{
    BitSequence* casted = dynamic_cast<BitSequence*>(result);
    if (casted == nullptr)
    {
        delete result;
        throw std::runtime_error("Internal error: result is not BitSequence");
    }

    if (casted != target)
    {
        delete target;
        target = casted;
    }
}

void PrintIntSequence(const std::string& name, const Sequence<int>* seq)
{
    std::cout << name << ": ";
    if (seq == nullptr)
    {
        std::cout << "<not created>\n";
        return;
    }

    std::cout << "[";
    for (int i = 0; i < seq->GetLength(); ++i)
    {
        if (i > 0)
        {
            std::cout << ", ";
        }
        std::cout << seq->Get(i);
    }
    std::cout << "]\n";
}

void PrintBitSequence(const std::string& name, const BitSequence* seq)
{
    std::cout << name << ": ";
    if (seq == nullptr)
    {
        std::cout << "<not created>\n";
        return;
    }

    for (int i = 0; i < seq->GetLength(); ++i)
    {
        std::cout << (seq->Get(i).GetValue() ? '1' : '0');
    }
    std::cout << "\n";
}

void PrintMenu()
{
    std::cout << "\n===== MENU =====\n";
    std::cout << "1. Create MutableArraySequence<int>\n";
    std::cout << "2. Create ImmutableArraySequence<int>\n";
    std::cout << "3. Create ListSequence<int>\n";
    std::cout << "4. Print all int sequences\n";
    std::cout << "5. Append to mutable sequence\n";
    std::cout << "6. Append to immutable sequence\n";
    std::cout << "7. Append to list sequence\n";
    std::cout << "8. Prepend to mutable sequence\n";
    std::cout << "9. InsertAt to immutable sequence\n";
    std::cout << "10. GetSubsequence from list sequence\n";
    std::cout << "11. Map square on mutable sequence\n";
    std::cout << "12. Where even on list sequence\n";
    std::cout << "13. Reduce sum on immutable sequence\n";
    std::cout << "14. Concat mutable + list\n";
    std::cout << "15. Create BitSequence\n";
    std::cout << "16. Print BitSequence\n";
    std::cout << "17. Bit NOT\n";
    std::cout << "18. Bit AND with entered sequence\n";
    std::cout << "19. Bit OR with entered sequence\n";
    std::cout << "20. Bit XOR with entered sequence\n";
    std::cout << "21. Map NOT on BitSequence\n";
    std::cout << "22. Where(true) on BitSequence\n";
    std::cout << "23. Reduce XOR on BitSequence\n";
    std::cout << "0. Exit\n";
}

int main()
{
    Sequence<int>* mutableSeq = nullptr;
    Sequence<int>* immutableSeq = nullptr;
    Sequence<int>* listSeq = nullptr;
    BitSequence* bitSeq = nullptr;

    bool running = true;

    while (running)
    {
        try
        {
            PrintMenu();
            int cmd = ReadInt("Choose: ");

            switch (cmd)
            {
                case 1:
                {
                    int count = 0;
                    int* data = ReadIntArray(count);

                    delete mutableSeq;
                    mutableSeq = new MutableArraySequence<int>(data, count);

                    delete[] data;
                    std::cout << "MutableArraySequence<int> created.\n";
                    break;
                }

                case 2:
                {
                    int count = 0;
                    int* data = ReadIntArray(count);

                    delete immutableSeq;
                    immutableSeq = new ImmutableArraySequence<int>(data, count);

                    delete[] data;
                    std::cout << "ImmutableArraySequence<int> created.\n";
                    break;
                }

                case 3:
                {
                    int count = 0;
                    int* data = ReadIntArray(count);

                    delete listSeq;
                    listSeq = new ListSequence<int>(data, count);

                    delete[] data;
                    std::cout << "ListSequence<int> created.\n";
                    break;
                }

                case 4:
                {
                    PrintIntSequence("Mutable", mutableSeq);
                    PrintIntSequence("Immutable", immutableSeq);
                    PrintIntSequence("List", listSeq);
                    break;
                }

                case 5:
                {
                    if (mutableSeq == nullptr)
                    {
                        throw std::runtime_error("Create mutable sequence first");
                    }

                    int value = ReadInt("Append value: ");
                    ReplaceSequence(mutableSeq, mutableSeq->Append(value));
                    PrintIntSequence("Mutable", mutableSeq);
                    break;
                }

                case 6:
                {
                    if (immutableSeq == nullptr)
                    {
                        throw std::runtime_error("Create immutable sequence first");
                    }

                    int value = ReadInt("Append value: ");
                    ReplaceSequence(immutableSeq, immutableSeq->Append(value));
                    PrintIntSequence("Immutable", immutableSeq);
                    break;
                }

                case 7:
                {
                    if (listSeq == nullptr)
                    {
                        throw std::runtime_error("Create list sequence first");
                    }

                    int value = ReadInt("Append value: ");
                    ReplaceSequence(listSeq, listSeq->Append(value));
                    PrintIntSequence("List", listSeq);
                    break;
                }

                case 8:
                {
                    if (mutableSeq == nullptr)
                    {
                        throw std::runtime_error("Create mutable sequence first");
                    }

                    int value = ReadInt("Prepend value: ");
                    ReplaceSequence(mutableSeq, mutableSeq->Prepend(value));
                    PrintIntSequence("Mutable", mutableSeq);
                    break;
                }

                case 9:
                {
                    if (immutableSeq == nullptr)
                    {
                        throw std::runtime_error("Create immutable sequence first");
                    }

                    int value = ReadInt("Insert value: ");
                    int index = ReadInt("Index: ");
                    ReplaceSequence(immutableSeq, immutableSeq->InsertAt(value, index));
                    PrintIntSequence("Immutable", immutableSeq);
                    break;
                }

                case 10:
                {
                    if (listSeq == nullptr)
                    {
                        throw std::runtime_error("Create list sequence first");
                    }

                    int left = ReadInt("Start index: ");
                    int right = ReadInt("End index: ");

                    Sequence<int>* sub = listSeq->GetSubsequence(left, right);
                    PrintIntSequence("Subsequence", sub);
                    delete sub;
                    break;
                }

                case 11:
                {
                    if (mutableSeq == nullptr)
                    {
                        throw std::runtime_error("Create mutable sequence first");
                    }

                    Sequence<int>* mapped = mutableSeq->Map(square_int);
                    PrintIntSequence("Mapped mutable", mapped);
                    delete mapped;
                    break;
                }

                case 12:
                {
                    if (listSeq == nullptr)
                    {
                        throw std::runtime_error("Create list sequence first");
                    }

                    Sequence<int>* filtered = listSeq->Where(is_even_int);
                    PrintIntSequence("Filtered list", filtered);
                    delete filtered;
                    break;
                }

                case 13:
                {
                    if (immutableSeq == nullptr)
                    {
                        throw std::runtime_error("Create immutable sequence first");
                    }

                    int result = Reduce(*immutableSeq, sum_reduce_int, 0);
                    std::cout << "Reduce(sum) result = " << result << "\n";
                    break;
                }

                case 14:
                {
                    if (mutableSeq == nullptr || listSeq == nullptr)
                    {
                        throw std::runtime_error("Create mutable and list sequences first");
                    }

                    Sequence<int>* concat = mutableSeq->Concat(listSeq);
                    PrintIntSequence("Concat result", concat);
                    delete concat;
                    break;
                }

                case 15:
                {
                    int count = 0;
                    Bit* data = ReadBitArrayFromString(count);

                    delete bitSeq;
                    bitSeq = new BitSequence(data, count);

                    delete[] data;
                    std::cout << "BitSequence created.\n";
                    break;
                }

                case 16:
                {
                    PrintBitSequence("BitSequence", bitSeq);
                    break;
                }

                case 17:
                {
                    if (bitSeq == nullptr)
                    {
                        throw std::runtime_error("Create BitSequence first");
                    }

                    BitSequence* result = bitSeq->Not();
                    delete bitSeq;
                    bitSeq = result;
                    PrintBitSequence("BitSequence", bitSeq);
                    break;
                }

                case 18:
                {
                    if (bitSeq == nullptr)
                    {
                        throw std::runtime_error("Create BitSequence first");
                    }

                    int count = 0;
                    Bit* data = ReadBitArrayFromString(count);
                    BitSequence other(data, count);
                    delete[] data;

                    BitSequence* result = bitSeq->And(other);
                    delete bitSeq;
                    bitSeq = result;
                    PrintBitSequence("BitSequence", bitSeq);
                    break;
                }

                case 19:
                {
                    if (bitSeq == nullptr)
                    {
                        throw std::runtime_error("Create BitSequence first");
                    }

                    int count = 0;
                    Bit* data = ReadBitArrayFromString(count);
                    BitSequence other(data, count);
                    delete[] data;

                    BitSequence* result = bitSeq->Or(other);
                    delete bitSeq;
                    bitSeq = result;
                    PrintBitSequence("BitSequence", bitSeq);
                    break;
                }

                case 20:
                {
                    if (bitSeq == nullptr)
                    {
                        throw std::runtime_error("Create BitSequence first");
                    }

                    int count = 0;
                    Bit* data = ReadBitArrayFromString(count);
                    BitSequence other(data, count);
                    delete[] data;

                    BitSequence* result = bitSeq->Xor(other);
                    delete bitSeq;
                    bitSeq = result;
                    PrintBitSequence("BitSequence", bitSeq);
                    break;
                }

                case 21:
                {
                    if (bitSeq == nullptr)
                    {
                        throw std::runtime_error("Create BitSequence first");
                    }

                    Sequence<Bit>* mapped = bitSeq->Map(invert_bit);
                    ReplaceBitSequence(bitSeq, mapped);
                    PrintBitSequence("BitSequence", bitSeq);
                    break;
                }

                case 22:
                {
                    if (bitSeq == nullptr)
                    {
                        throw std::runtime_error("Create BitSequence first");
                    }

                    Sequence<Bit>* filtered = bitSeq->Where(is_true_bit);
                    PrintBitSequence("Filtered result", dynamic_cast<BitSequence*>(filtered));
                    delete filtered;
                    break;
                }

                case 23:
                {
                    if (bitSeq == nullptr)
                    {
                        throw std::runtime_error("Create BitSequence first");
                    }

                    Bit result = Reduce(*bitSeq, xor_reduce_bit, Bit(false));
                    std::cout << "Reduce(XOR) result = " << (result.GetValue() ? 1 : 0) << "\n";
                    break;
                }

                case 0:
                {
                    running = false;
                    break;
                }

                default:
                {
                    std::cout << "Unknown command.\n";
                    break;
                }
            }
        }
        catch (const std::exception& ex)
        {
            std::cout << "Error: " << ex.what() << "\n";
        }
    }

    delete mutableSeq;
    delete immutableSeq;
    delete listSeq;
    delete bitSeq;

    return 0;
}
