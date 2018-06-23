#ifndef _PN_REPLACE_H_
#define _PN_REPLACE_H_


char *strReplace(const char *src, const char *from, const char *to);

/* End of Forward Declaration */



/*
 *
 * Description:
 *    Find and replace text within a LoadRunner string.
 *
 * Parameters:
 *    param (in)    - pointer to LoadRunner Parameter Name
 *    from (in)    - pointer to text top search for
 *    to (in) - pointer to text to use as replacement
 *
 * Returns:
 *    Returns an integer. 0=Error, 1=Success.
 *
 * Example:
 *    lr_save_string("Hello World!", "Parameter");
 *    lr_replace("Parameter, " ", "+");
 *    lr_output_message("%s", lr_eval_string("{Parameter}"));
 *
 */
int lr_replace(const char *param, char *from, char *to)
{
    int res = 0;
    char *result;
    char buffer[1024];

    /*
     * Finalize the LR Param Name
     */
    sprintf(buffer, "{%s}", param);

    /*
     * Do the Search and Replace
     */
    result = strReplace(lr_eval_string(buffer), from, to);

    /*
     * Process results
     */
    if (result != NULL)
    {
        lr_save_string(result, param);
        free(result);
        res = 1;
    }
    return res;
}


/* 
 * Description:
 *    Search for and replace text within a string.
 *
 * Parameters:
 *    src (in)  - pointer to source string
 *    from (in) - pointer to search text
 *    to (in)   - pointer to replacement text
 *
 * Returns:
 *    Returns a pointer to dynamically-allocated memory containing string
 *    with occurences of the text pointed to by 'from' replaced by with
 *    the text pointed to by 'to'.
 *
 * Notes:
 *    Do not use this directly in scripts unless you are a more advanced
 *    user who knows C and string handling. See below for the function you
 *    should use!
 */
char *strReplace(const char *src, const char *from, const char *to)
{
    int size;
    int fromlen;
    int tolen;
    
    char *dst;
    char *buffer;
    char *match;
    
    /*
    * Find out the lengths of the source string, text to replace, and
    * the replacement text.
    */
    size = strlen(src) + 1;
    fromlen = strlen(from);
    tolen = strlen(to);

    /*
    * Allocate the first chunk with enough for the original string.
    */
    buffer = (char *)malloc(size);

    /*
    * We need to return 'buffer', so let's make a copy to mess around with.
    */
    dst = buffer;

    /*
    * Before we begin, let's see if malloc was successful.
    */
    if (buffer != NULL)
    {
        /*
         * Loop until no matches are found.
         */
        for (;;)
        {
            /*
             * Try to find the search text.
             */
            match = (char *) strstr(src, from);
            if (match != NULL)
            {
                /*
                 * Found search text at location 'match'.
                 * Find out how many characters to copy up to the 'match'.
                 */
                size_t count = match - src;

                /*
                 * We are going to realloc, and for that we will need a
                 * temporary pointer for safe usage.
                 */
                char *temp;

                /*
                 * Calculate the total size the string will be after the
                 * replacement is performed.
                 */
                size += tolen - fromlen;

                /*
                 * Attempt to realloc memory for the new size.
                 */
                temp = (char *)realloc(buffer, size);

                if (temp == NULL)
                {
                    /*
                     * Attempt to realloc failed. Free the previously malloc'd
                     * memory and return with our tail between our legs.
                     */
                    free(buffer);
                    return NULL;
                }

                /*
                 * The call to realloc was successful. But we'll want to
                 * return 'buffer' eventually, so let's point it to the memory
                 * that we are now working with. And let's not forget to point
                 * to the right location in the destination as well.
                 */
                dst = temp + (dst - buffer);
                buffer = temp;

                /*
                 * Copy from the source to the point where we matched. Then
                 * move the source pointer ahead by the amount we copied. And
                 * move the destination pointer ahead by the same amount.
                 */
                memmove(dst, src, count);
                src += count;
                dst += count;

                /*
                 * Now copy in the replacement text 'to' at the position of
                 * the match. Adjust the source pointer by the text we replaced.
                 * Adjust the destination pointer by the amount of replacement
                 * text.
                 */
                memmove(dst, to, tolen);
                src += fromlen;
                dst += tolen;
            }
            else
            {
                /*
                 * Copy any remaining part of the string. This includes the null
                 * termination character.
                 */
                strcpy(dst, src);
                break;
            }
        } /* End of for loop */
    }
    return buffer;
}

#endif /* _PN_REPLACE_H_ */
