struct LinkedList
{
	void* value;
	struct LinkedList* next;
};
struct LinkedList* next(struct LinkedList* l)
{
	return l->next;
}
struct LinkedList* add(struct LinkedList* first,void* v)
{
	struct LinkedList *l;
	l->value=v;
	l->next=first;
	return l;
}
