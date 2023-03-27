Code Review comments for the step2/CodeReview.cpp file:

Comments:
=========

1) The error handling strategy for this code is not considering memory release. As we can see below:

    hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
    if (FAILED(hr))
    {
        std::cout << "Failed to create WSCProductList object. ";
        return false;
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr))
    {
        std::cout << "Failed to query antivirus product list. ";
        return false;
    }

    If an error occurs, the memory allocated for PtrProductList is not being deallocated. 
  It can cause memory leaks.
    To solve this issue, you should call the Release() method before leaving the function;

2)  The PtrProductList object is not being released at the end of function.

3) The error messages, should be directed to stderr, as we can find on the following code:
    std::cout << "Failed to create WSCProductList object. "; 

    You could alternatively use DBG macro to send messages to VS output panel.

4) I would say that a best practice would be to put a comment section immediatelly
    over the function definition to help other people on your team to understand
   what this function is supposed to do, instead of needing to read the all the
   code. It could occasionally tell about the parameters, and if they are 
   input or output parameters. This may save time of your team.

5) I think the size of this routine is fine, and the rules for defining
   variable names are consistent. 
   I don't know the code standard being used on this project, but if
   you use the format below, will reduce vertical code density, making
   easier for the developers to understand the code:

   Use this:
        if (FAILED(hr)) {
            std::cout << "Failed to query AV product.";
            continue;
        }
   Instead of this:
        if (FAILED(hr))
        {
            std::cout << "Failed to query AV product.";
            continue;
        }
   Will reduce the number of lines of your function (Vertical code density), and
   will make it easier to navigate through the code.


6) For the error handling strategy you could have a specific
   function on the code to treat the error messages, so you
   cold change the behaviour easily.
   You could create a log routine, so the messages could be
   reviewed in case you have problems on your software, and
   it could be disabled as you need on production environment.


