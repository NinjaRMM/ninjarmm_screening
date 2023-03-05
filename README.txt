Submission by: Jesus Viveros Delgado
email: jesus.viveros.delgado@gmail.com

Notes:

step1:  
        - The methods in the class Job can be also pure virtual and move the responsability of implement them to the child classes,
          but I didn't do it since what was expected from these functions was already known from the requirements, so there's no need
          to duplicate these implementations in the child classes.
        - I added some extra prints that were not requested but made the output clear


step2:  I didn't create a subfolder for step2 since it was a code review. Usually I wouldn't commit the changes for the review but rather suggest them to the original author
        About the review itself:
        - I would suggest changing the return type from bool to a standardized error result object similar to HRESULT.
        - I left a TODO to consider removing the check for an empty query since with it the return "false" of the function means two things: 
                either the query succeeded, and there's no AV software or
                the query failed due to API call failure.

          these 2 cases should not be handled in the same way hence my suggestion. Nevertheless, it cannot be done without reviewing the calls to this function from external code.

        - I replaced the structure instantiation and copy into the map with and emplace to avoid unnecessary copies.

step3: no comments about it.