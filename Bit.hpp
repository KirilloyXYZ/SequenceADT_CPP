#ifndef BIT_HPP
#define BIT_HPP

class Bit
{
private:
    bool value;

public:
    Bit(bool value = false);

    bool GetValue() const;

    Bit operator&(const Bit& other) const;
    Bit operator|(const Bit& other) const;
    Bit operator^(const Bit& other) const;
    Bit operator~() const;
};

Bit::Bit(bool value)
{
    this->value = value;
}

bool Bit::GetValue() const
{
    return this->value;
}

Bit Bit::operator&(const Bit& other) const
{
    return Bit(this->value && other.value);
}

Bit Bit::operator|(const Bit& other) const
{
    return Bit(this->value || other.value);
}

Bit Bit::operator^(const Bit& other) const
{
    return Bit(this->value != other.value);
}

Bit Bit::operator~() const
{
    return Bit(!this->value);
}

#endif