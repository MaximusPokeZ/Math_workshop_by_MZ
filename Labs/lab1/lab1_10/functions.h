



// ll integer_from_n_radix(char *string, int radix, enum error_type* error_return, char (*char_to_int)(char), bool (*is_char_correct)(char, int))
// {
//     ll result = 0, multiplier = 1;
//     bool has_sign_entered = false, is_negative = false;
//     int length = strnlen_s(string, 64);

//     for(int i = length - 1; i >= 0; --i)
//     {
//         if (is_char_correct(string[i], radix) && !has_sign_entered)
//         {
//             result += char_to_int(string[i]) * multiplier;
//             multiplier *= radix;
//         } else if ((string[i] == '+' || string[i] == '-') && !has_sign_entered)
//         {
//             is_negative = string[i] == '-';
//             has_sign_entered = true;
//         } else
//         {
//             *error_return = error;
//             return 0;
//         }
//     }
//     *error_return = correct;
//     return is_negative ? result * -1 : result;