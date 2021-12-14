#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

struct node {
	char phone[16];
	uint32_t child[128];
};

struct node read_node(int fd, uint32_t ind) {
	// [16 byte] [4 byte] ...... [4 byte] - 132 byte
	lseek(fd, sizeof(struct node) * ind, SEEK_SET);
	struct node A;
	int flag = read(fd, &A, sizeof(struct node));
	if (flag != sizeof(struct node)) {
		exit(1);
	}
	return A;
}

void write_node(int fd, uint32_t ind, struct node* node) {
	// [16 byte] [4 byte] ...... [4 byte] - 132 byte
	lseek(fd, sizeof(struct node) * ind, SEEK_SET);
	int flag = write(fd, node, sizeof(struct node));
	if (flag != sizeof(struct node)) {
		exit(1);
	}
}

enum state {
	READ_NAME,
	READ_NUMBER,
};

int main() {
	unsigned char c; 
	enum state state = READ_NAME;
	int flag = 0;

	//  name \t number \n
	//  ...
	int file = open("./tree", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	int cnt = 0;
	int cur = 0;
	int number_symbol = 0;
	struct node node = {};
	while((flag = read(STDIN_FILENO, &c, sizeof(c))) > 0) {
		if (state == READ_NAME && c == '\t') {
			state = READ_NUMBER;
			continue;
		}
		if (state == READ_NUMBER && c == '\n') {
			write_node(file, cur, &node);
			state = READ_NAME;
			continue;
		}
		if (state == READ_NAME) {

			if (node.child[c] == 0) {
				struct node child = {};
				node.child[c] = ++cnt;
				write_node(file, cur, &node);
				node = child;
				cur = cnt;
			} else {
				struct node child = read_node(file, node.child[c]);
				cur = node.child[c];
				node = child;
			}
		} else if (state == READ_NUMBER) {
			node.phone[number_symbol++] = c;
		}

	}
	if (flag < 0) {
		return 1;
	}


}

