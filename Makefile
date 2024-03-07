# Webserv
NAME			=	webserv

WEB_SRC			=	src/cgi.cpp \
					src/utils.cpp \
					src/webserv.cpp \
					src/getFile.cpp \
					src/parseReq.cpp \
					src/contentType.cpp \
					src/model/Server.cpp \
					src/model/Socket.cpp \
					src/model/Request.cpp \
					src/model/PostReq.cpp \
					src/model/Location.cpp \
					src/configFile/parse.cpp \
					src/configFile/splitDirectives.cpp \
					src/response/Get.cpp \
					src/response/Post.cpp \
					src/response/Delete.cpp \
					src/response/Response.cpp \
					src/response/DefaultPages.cpp \

OBJS			=	$(WEB_SRC:.cpp=.o)


# Compiler
CXX				=	c++
RM				=	rm -f
CXXFLAGS		=	-Wall -Werror -Wextra -std=c++98 #-g3 -fsanitize=address

# Colours
RED				=	\033[0;31m
GREEN			=	\033[0;32m
YELLOW			=	\033[0;33m
BLUE			=	\033[0;34m
PURPLE			=	\033[0;35m
CYAN			=	\033[0;36m
WHITE			=	\033[0;37m
RESET			=	\033[0m

# Rules
all:		$(NAME)
			@printf "$(BLUE)==> $(CYAN)Webserv compiled ✅\n\n$(RESET)"


$(NAME):	$(OBJS)
			@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
			@$(RM) $(OBJS)
			@printf "\n$(BLUE)==> $(RED)Removed Webserv 🗑️\n$(RESET)"

fclean:		clean
			@$(RM) $(NAME)

re:			fclean all
			@printf "$(BLUE)==> $(CYAN)Webserv recompiled 🔄\n$(RESET)"

docker:
			@docker build --rm -t nginx-test .
			@docker run --rm -it -p 80:8080 nginx-test

.PHONY:		all clean fclean re docker
