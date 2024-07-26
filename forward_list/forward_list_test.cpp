#include <iostream>

#include "forward_list.hpp"

//TEST
int main(){
	
	using namespace std;
	using namespace ilrd;
	
	size_t list_size;
	ForwardList mylist;
	
	list_size = mylist.size();
	list_size == 0 ? cout<<"1. Good\n" : cout<<"1. bad\n";
	mylist.empty() == true ? cout<<"2. Good\n" : cout<<"2. bad\n";
	
	mylist.push_front(10);
	list_size = mylist.size();
	list_size == 1 ? cout<<"3. Good\n" : cout<<"3. bad\n";
	mylist.empty() == false ? cout<<"4. Good\n" : cout<<"4. bad\n";

	mylist.push_front(15);
	list_size = mylist.size();
	list_size == 2 ? cout<<"5. Good\n" : cout<<"5. bad\n";
	mylist.empty() == false ? cout<<"6. Good\n" : cout<<"6. bad\n";

	mylist.push_front(-20);
	list_size = mylist.size();
	list_size == 3 ? cout<<"7. Good\n" : cout<<"7. bad\n";
	mylist.empty() == false ? cout<<"8. Good\n" : cout<<"8. bad\n";
	
	mylist.push_front(4);
	list_size = mylist.size();
	list_size == 4 ? cout<<"9. Good\n" : cout<<"9. bad\n";
	mylist.empty() == false ? cout<<"10. Good\n" : cout<<"10. bad\n";
	
	mylist.push_front(11);
	list_size = mylist.size();
	list_size == 5 ? cout<<"11. Good\n" : cout<<"11. bad\n";
	mylist.empty() == false ? cout<<"12. Good\n" : cout<<"12. bad\n";		
	
	mylist.pop_front();
	list_size = mylist.size();
	list_size == 4 ? cout<<"12. Good\n" : cout<<"12. bad\n";
	mylist.empty() == false ? cout<<"13. Good\n" : cout<<"13. bad\n";
	
	mylist.pop_front();
	list_size = mylist.size();
	list_size == 3 ? cout<<"14. Good\n" : cout<<"14. bad\n";
	mylist.empty() == false ? cout<<"15. Good\n" : cout<<"15. bad\n";
	
	mylist.pop_front();
	list_size = mylist.size();
	list_size == 2 ? cout<<"16. Good\n" : cout<<"16. bad\n";
	mylist.empty() == false ? cout<<"17. Good\n" : cout<<"17. bad\n";
	
	mylist.pop_front();
	list_size = mylist.size();
	list_size == 1 ? cout<<"18. Good\n" : cout<<"18. bad\n";
	mylist.empty() == false ? cout<<"19. Good\n" : cout<<"19. bad\n";
	
	mylist.pop_front();
	list_size = mylist.size();
	list_size == 0 ? cout<<"20. Good\n" : cout<<"20. bad\n";
	mylist.empty() == true ? cout<<"21. Good\n" : cout<<"21. bad\n";
	
	mylist.push_front(20);
	list_size = mylist.size();
	list_size == 1 ? cout<<"22. Good\n" : cout<<"22. bad\n";
	mylist.empty() == false ? cout<<"23. Good\n" : cout<<"23. bad\n";
	
	mylist.push_front(-7);
	list_size = mylist.size();
	list_size == 2 ? cout<<"24. Good\n" : cout<<"24. bad\n";
	mylist.empty() == false ? cout<<"25. Good\n" : cout<<"25. bad\n";	
	
	ForwardList mylist_cpy(mylist);
	list_size = mylist_cpy.size();
	list_size == 2 ? cout<<"26. Good\n" : cout<<"26. bad\n";
	mylist_cpy.empty() == false ? cout<<"27. Good\n" : cout<<"27. bad\n";
	mylist_cpy.front() == mylist.front() ? cout<<"28. Good\n" : cout<<"28. bad\n";
	
	ForwardList mylist_ass;
	mylist_ass = mylist_cpy;
	list_size = mylist_ass.size();
	list_size == 2 ? cout<<"29. Good\n" : cout<<"29. bad\n";
	mylist_ass.empty() == false ? cout<<"30. Good\n" : cout<<"30. bad\n";
	mylist_cpy.front() == mylist.front() ? cout<<"31. Good\n" : cout<<"31. bad\n";
	
	cout << "Do not forget to check valgrind..." << endl;

	return (0);
}
