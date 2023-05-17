#include "StreamingDBa1.h"

streaming_database::streaming_database() : movieTree(), userTree(), groupTree()
{
	// TODO: Your code goes here
}

streaming_database::~streaming_database()
{
	// TODO: Your code goes here
}


StatusType streaming_database::add_movie(int movieId, Genre genre, int views, bool vipOnly)
{
    if(genre == Genre::NONE || movieId <= 0 || views < 0)
        return StatusType::INVALID_INPUT;
    movieData data(movieId, genre, views, vipOnly);
    try
    {
        movieTree.insert(data);
    }
    catch(std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR; 
    }
    catch(Failure& e)
    {
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::remove_movie(int movieId)
{
    if(movieId <= 0)
        return StatusType::INVALID_INPUT;
    try
    {
        movieTree.remove(movieId);
    }
    catch(std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    catch(Failure& e)
    {
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::add_user(int userId, bool isVip)
{
    if(userId <= 0)
        return StatusType::INVALID_INPUT;
    userData data(userId, isVip);
    try
    {
        userTree.insert(data);
    }
    catch(std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    catch(Failure& e)
    {
        return StatusType::FAILURE;
    }
	return StatusType::SUCCESS;
}

StatusType streaming_database::remove_user(int userId)
{
    if(userId <= 0)
        return StatusType::INVALID_INPUT;
    try
    {
        userTree.remove(userId);
    }
    catch(std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    catch(Failure& e)
    {
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::add_group(int groupId)
{
    if(groupId <= 0)
        return StatusType::INVALID_INPUT;
    groupData group(groupId);
    try
    {
        groupTree.insert(group);
    }
    catch(std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    catch(Failure& e)
    {
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::remove_group(int groupId)
{
    if(groupId <= 0)
        return StatusType::INVALID_INPUT;
    try
    {
        groupTree.remove(groupId);
    }
    catch(std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    catch(Failure& e)
    {
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::add_user_to_group(int userId, int groupId)
{
    if(userId <=0 || groupId <= 0)
        return StatusType::INVALID_INPUT;
    try
    {
        groupData* group = (groupTree.findNode(groupId))->data;
        userData user = *((userTree.findNode(userId))->data);
        group->add_user(user);
    }
    catch(Failure& e)
    {
        return StatusType::FAILURE;
    }
    catch(std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    
    return StatusType::SUCCESS;
}

StatusType streaming_database::user_watch(int userId, int movieId)
{
    if(userId <=0 || movieId <= 0)
        return StatusType::INVALID_INPUT;
    try
    {
        movieData* movie = (movieTree.findNode(movieId))->data;
        userData* user = (userTree.findNode(userId))->data;
        if(movie->vipOnly && !(user->vipStatus))
            return StatusType::FAILURE;
        (movie->views)++;
    }
    catch(Failure& e)
    {
        return StatusType::FAILURE;
    }
    catch(std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    
    return StatusType::SUCCESS;
}

StatusType streaming_database::group_watch(int groupId,int movieId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_all_movies_count(Genre genre)
{
    // TODO: Your code goes here
    static int i = 0;
    return (i++==0) ? 11 : 2;
}

StatusType streaming_database::get_all_movies(Genre genre, int *const output)
{
    // TODO: Your code goes here
    output[0] = 4001;
    output[1] = 4002;
    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_num_views(int userId, Genre genre)
{
	// TODO: Your code goes here
	return 2008;
}

StatusType streaming_database::rate_movie(int userId, int movieId, int rating)
{
    // TODO: Your code goes here
    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_group_recommendation(int groupId)
{
	// TODO: Your code goes here
    static int i = 0;
    return (i++==0) ? 11 : 2;
}


